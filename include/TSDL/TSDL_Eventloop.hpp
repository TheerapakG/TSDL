#ifndef TSDL_EVENTLOOP_
#define TSDL_EVENTLOOP_

#include <SDL.h>
#include <map>
#include <chrono>
#include <atomic>
#include <mutex>
#include <functional>

#ifdef TSDL_EXPOSE_PYBIND11
#include "TSDL_PY_TypeErase.hpp"
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
    using EventHandler = void(const SDL_Event&);

    using RenderHandler = std::function<void()>;

    using namespace std::literals::chrono_literals;

    class TSDL_Eventloop
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
        void _run_step();

        public:

        _PY_DECLARE_TYPEERASE_OWNER(Eventloop)

        TSDL_Eventloop();
        #ifdef __cpp_exceptions
        TSDL_Eventloop(bool thrownoevhandler, bool thrownorenderhandler);
        #endif

        ~TSDL_Eventloop();

        void add_event_handler(SDL_EventType evType, EventHandler handler);

        void remove_event_handler(SDL_EventType evType);

        void render_function(RenderHandler handler);

        const RenderHandler& render_function();

        void run();

        void interrupt();

        void track_fps(bool track = true);

        void fps_update_interval(clock::duration interval);

        clock::duration fps_update_interval() const;

        double fps() const;

        void limit_fps(bool limit = true);

        void fps_target(double frames);

        double fps_target() const;
    };
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