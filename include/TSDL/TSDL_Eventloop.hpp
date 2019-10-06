#ifndef TSDL_EVENTLOOP_
#define TSDL_EVENTLOOP_

#include <SDL.h>
#include <map>
#include <chrono>
#include <atomic>
#include <mutex>

typedef void (*EventHandler)(SDL_Event);

typedef void (*RenderHandler)(void);

namespace TSDL
{
    using namespace std::literals::chrono_literals;

    class TSDL_Eventloop
    {
        public:
        using clock = std::chrono::steady_clock;

        private:
        std::map <SDL_EventType, EventHandler> _map;
        RenderHandler _render = nullptr;
        std::atomic<bool> _is_running = false;
        bool _throw_if_no_event_handler = false;
        bool _throw_if_no_render_handler = false;

        std::atomic<bool> _track_fps = true;
        std::atomic<clock::duration> _fps_update_interval = 1s;
        std::recursive_mutex _lock_frame_calc;
            int _frame_since_last = 0;
            clock::time_point _time_since_last;
        std::atomic<double> _previous_fps = 0;

        std::atomic<bool> _limit_fps = true;
        std::atomic<clock::duration> _fps_target_interval = std::chrono::duration_cast<std::chrono::nanoseconds>(1s)/60;
        clock::time_point _time_last_frame;

        void _reset_fps_count();
        void _run_step();

        public:
        TSDL_Eventloop();
        TSDL_Eventloop(bool thrownoevhandler, bool thrownorenderhandler);

        ~TSDL_Eventloop();

        void add_event_handler(SDL_EventType evType, EventHandler handler);

        void remove_event_handler(SDL_EventType evType);

        void set_render_function(RenderHandler handler);

        void run();

        void interrupt();

        void track_fps(bool track = true);

        void set_fps_update_interval(clock::duration interval);

        clock::duration get_fps_update_interval() const;

        double get_fps() const;

        void limit_fps(bool limit = true);

        void set_fps_target(double frames);

        double get_fps_target() const;
    };
}

#endif