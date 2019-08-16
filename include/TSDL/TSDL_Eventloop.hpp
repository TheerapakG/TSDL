#ifndef TSDL_EVENTLOOP_
#define TSDL_EVENTLOOP_

#include <SDL.h>
#include <map>

typedef void (*EventHandler)(SDL_Event);

typedef void (*RenderHandler)(void);

namespace TSDL
{
    class TSDL_Eventloop
    {
        private:
        std::map <SDL_EventType, EventHandler> _map;
        RenderHandler _render = nullptr;
        bool _is_running = false;
        bool _throw_if_no_event_handler = false;
        bool _throw_if_no_render_handler = false;

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
    };
}

#endif