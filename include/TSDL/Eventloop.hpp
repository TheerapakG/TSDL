//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_EVENTLOOP_
#define TSDL_EVENTLOOP_

#include <SDL2/SDL.h>
#include <exception>
#include <map>
#include <chrono>
#include <atomic>
#include <future>
#include <mutex>
#include <functional>

#ifdef TSDL_EXPOSE_PYBIND11
#include "PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Eventloop)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Eventloop)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Eventloop)
    _PY_GET_CONTEXTMANAGER(Eventloop)<>,
    _PY_GET_CONTEXTMANAGER(Eventloop)<bool, bool>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    using EventHandler = std::function<void(const SDL_Event&)>;

    using RenderHandler = std::function<void()>;

    using namespace std::literals::chrono_literals;

    namespace impl
    {
        template <typename T>
        get_tuple_t<
            std::promise<T>, 
            std::function<void(void)>
        > 
        _bind_function_promise(const std::function<T(void)>& func)
        {
            std::promise<T> _promise;
            std::function<void(void)> _executing_func = [func, &_promise]() -> void
            {
                try
                {
                    _promise.set_value(func());
                }
                catch(...)
                {
                    _promise.set_exception(std::current_exception());
                }
            };
            return make_tuple(std::move(_promise), std::move(_executing_func));
        }

        template <>
        get_tuple_t<
            std::promise<void>, 
            std::function<void(void)>
        > 
        _bind_function_promise<void>(const std::function<void(void)>& func);
    }

    class Eventloop
    {
        public:
        using clock = std::chrono::steady_clock;

        private:
        std::map <SDL_EventType, EventHandler> _map;
        RenderHandler _render = nullptr;
        std::atomic<bool> _is_running = false;
        #ifdef __cpp_exceptions
        bool _throw_if_no_event_handler = false;
        bool _throw_if_no_render_handler = false;
        #endif

        mutable std::mutex _m_func_vector;
        std::vector<std::function<void(void)>> _pending_func, _running_func;

        clock::time_point _now;

        std::atomic<bool> _track_fps = true;
        std::atomic<clock::duration> _fps_update_interval = std::chrono::duration_cast<clock::duration>(1s);
        std::recursive_mutex _lock_frame_calc;
            int _frame_since_last = 0;
            clock::time_point _time_since_last;
        std::atomic<double> _previous_fps = 0;

        std::atomic<bool> _limit_fps = false;
        std::atomic<clock::duration> _fps_target_interval = std::chrono::duration_cast<std::chrono::nanoseconds>(1s)/60;
        clock::time_point _time_last_frame;

        void _reset_fps_count();

        void _handle_event();
        void _handle_func();
        void _handle_render();
        void _run_step();

        template <typename... Args>
        void _push_back_function_unlocked(std::function<void(void)>&& func, Args... args)
        {
            _pending_func.push_back(func);
            _push_back_function_unlocked(std::forward<Args...>(args)...);
        }

        template <typename... Args>
        void _push_back_function(Args... args)
        {
            std::scoped_lock _lock(_m_func_vector);
            _push_back_function_unlocked(std::forward<Args...>(args)...);
        }

        template <typename... Args, size_t... I>
        void _push_back_function_tuple(get_tuple_t<Args...> args_tpl, std::index_sequence<I...>)
        {
            _push_back_function(std::get<I>(args_tpl)...);
        }

        template <typename... Args>
        void _push_back_function_tuple(get_tuple_t<Args...> args_tpl)
        {
            _push_back_function_tuple(args_tpl, std::make_index_sequence<sizeof...(Args)>{});
        }

        public:

        _PY_DECLARE_TYPEERASE_OWNER(Eventloop)

        Eventloop() = default;
        #ifdef __cpp_exceptions
        Eventloop(bool thrownoevhandler, bool thrownorenderhandler);
        #endif
        Eventloop(const Eventloop&) = delete;
        Eventloop& operator=(const Eventloop&) = delete;

        ~Eventloop();

        /*
        Add event handler
        not thread-safe
        */
        void add_event_handler(SDL_EventType evType, EventHandler handler);

        /*
        Remove event handler
        not thread-safe
        */
        void remove_event_handler(SDL_EventType evType);

        /*
        Set render function
        not thread-safe
        */
        void render_function(RenderHandler handler);

        /*
        Get render function
        not thread-safe
        */
        const RenderHandler& render_function();

        /*
        Start the eventloop
        not thread-safe

        Note: run this from the main thread only!
        */
        void run();

        /*
        Stop the event loop.
        thread-safe
        */
        void interrupt();

        /*
        Return the time that last frame is drawn.
        not thread-safe
        */
        clock::time_point now();

        /*
        Set whether the loop will track fps.
        thread-safe
        */
        void track_fps(bool track = true);

        /*
        Set the interval that the fps counter will be updated.
        thread-safe
        */
        void fps_update_interval(clock::duration interval);

        /*
        Get the interval that the fps counter will be updated.
        thread-safe
        */
        clock::duration fps_update_interval() const;

        /*
        Get current fps.
        thread-safe
        */
        double fps() const;

        /*
        Set whether fps be limited or not.
        thread-safe
        */
        void limit_fps(bool limit = true);

        /*
        Set fps limit target.
        thread-safe
        */
        void fps_target(double frames);

        /*
        Get fps limit target.
        thread-safe
        */
        double fps_target() const;

        template <typename T>
        std::promise<T> execute_next_cycle(std::function<T(void)> func)
        {
            auto [_promise, _executing_func] = impl::_bind_function_promise(func);
            _push_back_function(_executing_func);
            return _promise;
        }

        template <typename... Args>
        get_tuple_t<std::promise<Args>...> batch_execute_next_cycle(std::function<Args(void)>... func)
        {
            auto [_promise_tuple, _executing_func_tuple] = std::apply(
                [](auto&&... _promise_execution_pair)
                {
                    return make_tuple(
                        make_tuple(std::get<0>(_promise_execution_pair)...), 
                        make_tuple(std::get<1>(_promise_execution_pair)...)
                    );
                },
                make_tuple(impl::_bind_function_promise(func)...)
            );
            _push_back_function_tuple(_executing_func_tuple);
            return _promise_tuple;
        }
    };

    /*
    Get current running eventloop.
    thread-safe
    */
    Eventloop& current_eventloop();
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Eventloop)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Eventloop)
}

void _tsdl_eventloop_py(const py::module& m);

#endif

#endif