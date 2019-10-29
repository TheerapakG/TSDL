#include "TSDL/TSDL_Eventloop.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std::literals::chrono_literals;

TSDL::TSDL_Eventloop::TSDL_Eventloop() {}

TSDL::TSDL_Eventloop::TSDL_Eventloop(bool thrownoevhandler, bool thrownorenderhandler) :
_throw_if_no_event_handler(thrownoevhandler),
_throw_if_no_render_handler(thrownorenderhandler)
{}

TSDL::TSDL_Eventloop::~TSDL_Eventloop()
{
    if(_is_running.load())
    {
        this->interrupt();
    }
}

void TSDL::TSDL_Eventloop::add_event_handler(SDL_EventType evType, TSDL::EventHandler handler)
{
    _map[evType] = handler;
}

void TSDL::TSDL_Eventloop::remove_event_handler(SDL_EventType evType)
{
    _map.erase(evType);
}

void TSDL::TSDL_Eventloop::render_function(TSDL::RenderHandler handler)
{
    _render = handler;
}

void TSDL::TSDL_Eventloop::_reset_fps_count()
{
    std::scoped_lock lock(_lock_frame_calc);
    _frame_since_last = 0;
    _time_since_last = clock::now();
}

void TSDL::TSDL_Eventloop::_run_step()
{
    SDL_Event e;
    if(SDL_PollEvent(&e) != 0)
    {
        EventHandler h;
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
                return;
            }
        }
        h(e);
        return;
    }
    else
    {
        if(_render != nullptr)
        {
            if(_limit_fps.load())
            {
                clock::time_point now = clock::now();
                if((now - _time_last_frame) < _fps_target_interval.load()) return;
                _time_last_frame = now;
            }
            _render();
            if(_track_fps.load())
            {
                clock::time_point now = clock::now();
                std::scoped_lock lock(_lock_frame_calc);
                clock::duration diff = now - _time_since_last;
                if(diff >= _fps_update_interval.load())
                {
                    _previous_fps.store(static_cast<double>(_frame_since_last + 1) / std::chrono::duration_cast<std::chrono::seconds>(diff).count());
                    this->_reset_fps_count();
                }
                else _frame_since_last ++;
            }
            return;
        }
        else
        {
            if(_throw_if_no_render_handler)
            {
                std::throw_with_nested(std::runtime_error("No handler for rendering"));
            }
            else
            {
                std::cerr << "No handler for rendering." << std::endl;
                return;
            }
        }
        
    }
}

void TSDL::TSDL_Eventloop::run()
{
    if(_track_fps) this->_reset_fps_count();
    _time_last_frame = clock::now();
    _is_running.store(true);
    while(_is_running.load())
    {
        this->_run_step();
    }
}

void TSDL::TSDL_Eventloop::interrupt()
{
    if(_is_running)
    {
        _is_running.store(false);
    }
    else
    {
        throw std::runtime_error("Could not interrupt while event loop is not already running!");
    }
    
}

void TSDL::TSDL_Eventloop::track_fps(bool track)
{
    _track_fps.store(track);
    this->_reset_fps_count();
}

void TSDL::TSDL_Eventloop::fps_update_interval(TSDL::TSDL_Eventloop::clock::duration interval)
{
    _fps_update_interval.store(interval);
    this->_reset_fps_count();
}

TSDL::TSDL_Eventloop::clock::duration TSDL::TSDL_Eventloop::fps_update_interval() const
{
    return _fps_update_interval.load();
}

double TSDL::TSDL_Eventloop::fps() const
{
    return _previous_fps.load();
}

void TSDL::TSDL_Eventloop::limit_fps(bool limit)
{
    _limit_fps.store(limit);
}

void TSDL::TSDL_Eventloop::fps_target(double frames)
{
    _fps_target_interval.store(
        std::chrono::duration_cast<TSDL::TSDL_Eventloop::clock::duration>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(1s)/frames
        )
    );
}

double TSDL::TSDL_Eventloop::fps_target() const
{
    return static_cast<double>((1s).count())/_fps_update_interval.load().count();
}

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL/TSDL_PY_Utility.hpp"

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Eventloop)

void _tsdl_eventloop_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Eventloop)>(m, "Eventloop")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Eventloop)<>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Eventloop)<bool, bool>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Eventloop, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Eventloop, exit_ctx));
    py::class_<TSDL::TSDL_Eventloop>(m, "_Eventloop")
        .def("add_event_handler", 
            [](TSDL::TSDL_Eventloop* _self, SDL_EventType evType,  py::function handler)
            {
                _self->add_event_handler(evType, TSDL::py_function_cast_function_pointer<TSDL::EventHandler>(handler));
            }
        )
        .def("remove_event_handler", &TSDL::TSDL_Eventloop::remove_event_handler)
        .def("render_function",
            [](TSDL::TSDL_Eventloop* _self, py::function handler)
            {
                _self->render_function(TSDL::py_function_cast_function_pointer<TSDL::RenderHandler>(handler));
            }
        )
        .def("run", &TSDL::TSDL_Eventloop::run)
        .def("interrupt", &TSDL::TSDL_Eventloop::interrupt);
}

#endif