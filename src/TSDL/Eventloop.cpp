//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Eventloop.hpp"
#include "TSDL/Meta.hpp"
#include "TSDL/Macro.hpp"
#include "TSDL/Utility.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

#ifdef TSDL_USE_EMSCRIPTEN
    #include <emscripten.h>
#endif

using namespace std::literals::chrono_literals;

namespace TSDL::impl
{
    optional_reference<Eventloop> _current_eventloop;
    std::recursive_mutex _m_current_eventloop;

    template <>
    get_tuple_t<
        std::promise<void>, 
        std::function<void(void)>
    > 
    _bind_function_promise<void>(const std::function<void(void)>& func)
    {
        std::promise<void> _promise;
        std::function<void(void)> _executing_func = [func, &_promise]() -> void
        {
            try
            {
                func();
                _promise.set_value();
            }
            catch(...)
            {
                _promise.set_exception(std::current_exception());
            }
        };
        return make_tuple(std::move(_promise), std::move(_executing_func));
    }
}

TSDL::Eventloop::Eventloop(_pconstruct_t, bool thrownoevhandler, bool thrownorenderhandler)
{
    std::scoped_lock lock(impl::_m_current_eventloop);
    if (impl::_current_eventloop.has_value())
    {
        TSDL::safe_throw<std::runtime_error>("There is already an eventloop, running two eventloop simultaneously is not possible");
        return;
    }
    impl::_current_eventloop = *this;
}

TSDL::Eventloop::Eventloop(): Eventloop(pconstruct, false, false) {}

#ifdef __cpp_exceptions
TSDL::Eventloop::Eventloop(bool thrownoevhandler, bool thrownorenderhandler) :
Eventloop(pconstruct, thrownoevhandler, thrownorenderhandler) {}
#endif

TSDL::Eventloop::~Eventloop()
{
    if(_is_running.load())
    {
        this->interrupt();
    }
}

void TSDL::Eventloop::add_event_handler(SDL_EventType evType, TSDL::EventHandler handler)
{
    _map[evType] = handler;
}

void TSDL::Eventloop::remove_event_handler(SDL_EventType evType)
{
    _map.erase(evType);
}

void TSDL::Eventloop::render_function(TSDL::RenderHandler handler)
{
    _render = handler;
}

const TSDL::RenderHandler& TSDL::Eventloop::render_function()
{
    return _render;
}

void TSDL::Eventloop::_reset_fps_count()
{
    std::scoped_lock lock(_lock_frame_calc);
    _frame_since_last = 0;
    _time_since_last = clock::now();
}

void TSDL::Eventloop::_handle_event()
{
    SDL_Event e;
    _now = clock::now();
    while(SDL_PollEvent(&e) != 0)
    {
        EventHandler h;
        #ifdef __cpp_exceptions
        try
        {
            h = _map.at(static_cast<SDL_EventType>(e.type));
        }
        catch(const std::out_of_range& exc)
        {
            if(_throw_if_no_event_handler)
            {
                std::throw_with_nested(std::runtime_error("No handler for SDL event: " + std::to_string(e.type)));
            }
            else
            {
                std::cerr << "No handler for SDL event: " << e.type << " (" << exc.what() << "). You should consider adding handler." << std::endl;
                continue;
            }
        }
        #else
        auto h_it = _map.find(static_cast<SDL_EventType>(e.type));
        if _TSDL_LIKELY(h_it != _map.end())
        {
            h = h_it->second;
        }
        else
        {
            std::cerr << "No handler for SDL event: " << e.type << ". You should consider adding handler." << std::endl;
            return;
        }
        #endif
        h(e);
        continue;
    }
}

void TSDL::Eventloop::_handle_func()
{
    {
        std::scoped_lock _lock(_m_func_vector);
        std::swap(_running_func, _pending_func);
    }
    for(auto f: _running_func) f();
    _running_func.clear();
}

void TSDL::Eventloop::_handle_render()
{
    _now = clock::now();
    if(_render != nullptr)
    {
    #ifndef TSDL_USE_EMSCRIPTEN
        if(_limit_fps.load())
        {
            // limit time diff to be _fps_target_interval
            if _TSDL_LIKELY((_now - _time_last_frame) < _fps_target_interval.load()) return;
            _time_last_frame = _now;
        }
    #endif
        _render();
        if(_track_fps.load())
        {
            // increment _frame_since_last and also store to _previous_fps sometimes
            _now = clock::now();
            std::scoped_lock lock(_lock_frame_calc);
            clock::duration diff = _now - _time_since_last;
            if _TSDL_UNLIKELY(diff >= _fps_update_interval.load())
            {
                _previous_fps.store((static_cast<double>(_frame_since_last) + 1) / std::chrono::duration_cast<std::chrono::seconds>(diff).count());
                this->_reset_fps_count();
            }
            else _frame_since_last ++;
        }
        return;
    }
    else
    {
        #ifdef __cpp_exceptions
        if _TSDL_UNLIKELY(_throw_if_no_render_handler)
        {
            std::throw_with_nested(std::runtime_error("No handler for rendering"));
        }
        #endif

        std::cerr << "No handler for rendering." << std::endl;
        return;        
    }
}

void TSDL::Eventloop::_run_step()
{
    _handle_event();
    _handle_func();
    _handle_render();
}

void TSDL::Eventloop::run()
{
#ifndef TSDL_USE_EMSCRIPTEN
    if(_track_fps) this->_reset_fps_count();
    _time_last_frame = clock::now();
    _is_running.store(true);
    while(_is_running.load())
    {
        this->_run_step();
    }
#else
    if (_limit_fps.load())
    {
        emscripten_set_main_loop_arg(
            TSDL::ptr_fun<TSDL::Eventloop, void>::make_void<&TSDL::Eventloop::_run_step>::fun, 
            this, 
            static_cast<int>(1s/(_fps_target_interval.load())), 
            1
        );
    }
    else
    {
        emscripten_set_main_loop_arg(
            TSDL::ptr_fun<TSDL::Eventloop, void>::make_void<&TSDL::Eventloop::_run_step>::fun, 
            this, 
            -1, 
            1
        );
    }
#endif
}

void TSDL::Eventloop::interrupt()
{
#ifndef TSDL_USE_EMSCRIPTEN
    if(_is_running)
    {
        _is_running.store(false);
    }
    else
    {
        throw std::runtime_error("Could not interrupt while event loop is not already running!");
    }
#else
    emscripten_cancel_main_loop();
#endif
    std::scoped_lock lock(impl::_m_current_eventloop);
    impl::_current_eventloop.reset();
}

TSDL::Eventloop::clock::time_point TSDL::Eventloop::now()
{
    return _now;
}

void TSDL::Eventloop::track_fps(bool track)
{
    _track_fps.store(track);
    this->_reset_fps_count();
}

void TSDL::Eventloop::fps_update_interval(TSDL::Eventloop::clock::duration interval)
{
    _fps_update_interval.store(interval);
    this->_reset_fps_count();
}

TSDL::Eventloop::clock::duration TSDL::Eventloop::fps_update_interval() const
{
    return _fps_update_interval.load();
}

double TSDL::Eventloop::fps() const
{
    return _previous_fps.load();
}

void TSDL::Eventloop::limit_fps(bool limit)
{
    _limit_fps.store(limit);
#ifdef TSDL_USE_EMSCRIPTEN
    if (limit)
    {
        emscripten_set_main_loop_timing(
            EM_TIMING_SETTIMEOUT, 
            static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(_fps_target_interval.load()).count())
        );
    }
    else
    {
        emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
    }    
#endif
}

void TSDL::Eventloop::fps_target(double frames)
{
    _fps_target_interval.store(
        std::chrono::duration_cast<TSDL::Eventloop::clock::duration>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(1s)/frames
        )
    );
#ifdef TSDL_USE_EMSCRIPTEN
    if (_limit_fps.load())
    {
        emscripten_set_main_loop_timing(
            EM_TIMING_SETTIMEOUT, 
            static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(_fps_target_interval.load()).count())
        );
    }
    else
    {
        emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
    }  
#endif
}

double TSDL::Eventloop::fps_target() const
{
    return static_cast<double>((1s).count())/_fps_update_interval.load().count();
}

TSDL::Eventloop& TSDL::current_eventloop()
{
    std::scoped_lock lock(impl::_m_current_eventloop);
    return TSDL::impl::_current_eventloop.value();
}

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL/PY_Utility.hpp"

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Eventloop)

void _tsdl_eventloop_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Eventloop)>(m, "Eventloop")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Eventloop)<>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Eventloop)<bool, bool>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Eventloop, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Eventloop, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Eventloop, exit_ctx));
    py::class_<TSDL::Eventloop>(m, "_Eventloop")
        .def("add_event_handler", 
            [](TSDL::Eventloop* _self, SDL_EventType evType,  py::function handler)
            {
                _self->add_event_handler(evType, TSDL::py_function_cast_function_pointer<TSDL::EventHandler>(handler));
            }
        )
        .def("remove_event_handler", &TSDL::Eventloop::remove_event_handler)
        .def("render_function",
            [](TSDL::Eventloop* _self, py::function handler)
            {
                _self->render_function(TSDL::py_function_cast_function_pointer<TSDL::RenderHandler>(handler));
            }
        )
        .def("run", &TSDL::Eventloop::run)
        .def("interrupt", &TSDL::Eventloop::interrupt);
}

#endif