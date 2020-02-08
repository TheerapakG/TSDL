#include <iostream>
#include <optional>
#include <functional>
#include "abstract/elements/EventloopAdapter.hpp"
#include "TSDL_Meta.hpp"

using namespace std::placeholders;

namespace
{
    void _render(const TSDL::elements::EventloopAdapter& adapter)
    {
        TSDL::TSDL_Renderer& renderer = adapter.renderer();
        TSDL::elements::Element& src = adapter.src();

        if (!src.need_update()) return;
        src.render();
        src.not_update();
        renderer.copy_from(src.get_texture(), std::optional<TSDL::rect>(), std::optional<TSDL::rect>());
    }

    void _handle_window_event(TSDL::elements::EventloopAdapter& adapter, const ::SDL_Event& event)
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowShown, event
            );
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowHidden, event
            );
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowExposed, event
            );
            break;
        case SDL_WINDOWEVENT_MOVED:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowMoved, event
            );
            break;
        case SDL_WINDOWEVENT_RESIZED:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowResized, event
            );
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowMinimized, event
            );
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowMaximized, event
            );
            break;
        case SDL_WINDOWEVENT_RESTORED:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowRestored, event
            );
            break;
        case SDL_WINDOWEVENT_ENTER:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::MouseIn, event
            );
            break;
        case SDL_WINDOWEVENT_LEAVE:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::MouseOut, event
            );
            break;
        case SDL_WINDOWEVENT_CLOSE:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::WindowClose, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_keydown_event(TSDL::elements::EventloopAdapter& adapter, const ::SDL_Event& event)
    {
        adapter.src().dispatch_event(
            TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::KeyDown, event
        );
    }

    void _handle_keyup_event(TSDL::elements::EventloopAdapter& adapter, const ::SDL_Event& event)
    {
        adapter.src().dispatch_event(
            TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::KeyUp, event
        );
    }

    void _handle_textinput_event(TSDL::elements::EventloopAdapter& adapter, const ::SDL_Event& event)
    {
        adapter.src().dispatch_event(
            TSDL::elements::Caller(adapter, TSDL::point_2d(0, 0)), TSDL::events::TextInput, event
        );
    }

    void _handle_mousemotion_event(TSDL::elements::EventloopAdapter& adapter, const ::SDL_Event& event)
    {
        adapter.src().dispatch_event(
            TSDL::elements::Caller(adapter, TSDL::point_2d(event.motion.x, event.motion.y)), TSDL::events::MouseMotion, event
        );
    }

    void _handle_mousebuttondown_event(TSDL::elements::EventloopAdapter& adapter, const ::SDL_Event& event)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::LeftDown, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::MiddleDown, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::RightDown, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_mousebuttonup_event(TSDL::elements::EventloopAdapter& adapter, const ::SDL_Event& event)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            std::cout << event.button.x << " " << event.button.y << "\n";
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::LeftUp, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::MiddleUp, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter.src().dispatch_event(
                TSDL::elements::Caller(adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::RightUp, event
            );
            break;
        default:
            break;
        }
    }
}

TSDL::elements::EventloopAdapter::EventloopAdapter(TSDL_Renderer& renderer, TSDL_Eventloop& evloop, Element& src):
    EventDispatcher(renderer), _evloop(evloop), _src(src)
{
    if(_evloop.render_function()) 
    {
        TSDL::safe_throw<std::runtime_error>("Could not bind render adapter to an eventloop that already have render function");
        return;
    }
    _evloop.render_function(std::bind(_render, *this));
    _evloop.add_event_handler(
        SDL_WINDOWEVENT, callable_cast_function_pointer<TSDL::EventHandler>(std::bind(_handle_window_event, *this, _1))
    );
    _evloop.add_event_handler(
        SDL_KEYDOWN, callable_cast_function_pointer<TSDL::EventHandler>(std::bind(_handle_keydown_event, *this, _1))
    );
    _evloop.add_event_handler(
        SDL_KEYUP, callable_cast_function_pointer<TSDL::EventHandler>(std::bind(_handle_keyup_event, *this, _1))
    );
    _evloop.add_event_handler(
        SDL_TEXTINPUT, callable_cast_function_pointer<TSDL::EventHandler>(std::bind(_handle_textinput_event, *this, _1))
    );
    _evloop.add_event_handler(
        SDL_MOUSEMOTION, callable_cast_function_pointer<TSDL::EventHandler>(std::bind(_handle_mousemotion_event, *this, _1))
    );
    _evloop.add_event_handler(
        SDL_MOUSEBUTTONDOWN, callable_cast_function_pointer<TSDL::EventHandler>(std::bind(_handle_mousebuttondown_event, *this, _1))
    );
    _evloop.add_event_handler(
        SDL_MOUSEBUTTONUP, callable_cast_function_pointer<TSDL::EventHandler>(std::bind(_handle_mousebuttonup_event, *this, _1))
    );
}

TSDL::elements::EventloopAdapter::~EventloopAdapter()
{
    _evloop.render_function(nullptr);
}

TSDL::elements::Element& TSDL::elements::EventloopAdapter::src() const
{
    return _src;
}