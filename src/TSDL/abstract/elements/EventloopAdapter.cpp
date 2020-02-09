#include <iostream>
#include <optional>
#include <functional>
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

using namespace std::placeholders;

namespace
{
    void _render(const TSDL::elements::EventloopAdapter* adapter)
    {
        TSDL::TSDL_Renderer& renderer = adapter->renderer();
        TSDL::elements::Element& src = adapter->src();

        if (!src.need_update()) return;
        src.render();
        src.not_update();
        renderer.copy_from(
            src.get_texture(), 
            TSDL::make_optional_const_ref<TSDL::rect>(), 
            TSDL::make_optional_const_ref<TSDL::rect>()
        );
    }

    void _handle_window_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowShown, event
            );
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowHidden, event
            );
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowExposed, event
            );
            break;
        case SDL_WINDOWEVENT_MOVED:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowMoved, event
            );
            break;
        case SDL_WINDOWEVENT_RESIZED:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowResized, event
            );
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowMinimized, event
            );
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowMaximized, event
            );
            break;
        case SDL_WINDOWEVENT_RESTORED:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowRestored, event
            );
            break;
        case SDL_WINDOWEVENT_ENTER:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::MouseIn, event
            );
            break;
        case SDL_WINDOWEVENT_LEAVE:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::MouseOut, event
            );
            break;
        case SDL_WINDOWEVENT_CLOSE:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::WindowClose, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_keydown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::KeyDown, event
        );
    }

    void _handle_keyup_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::KeyUp, event
        );
    }

    void _handle_textinput_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), TSDL::events::EventType::TextInput, event
        );
    }

    void _handle_mousemotion_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(event.motion.x, event.motion.y)), TSDL::events::EventType::MouseMotion, event
        );
    }

    void _handle_mousebuttondown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::EventType::LeftDown, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::EventType::MiddleDown, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::EventType::RightDown, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_mousebuttonup_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::EventType::LeftUp, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::EventType::MiddleUp, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter->src().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), TSDL::events::EventType::RightUp, event
            );
            break;
        default:
            break;
        }
    }
}

TSDL::elements::EventloopAdapter::EventloopAdapter(TSDL_Renderer& renderer, TSDL_Eventloop& evloop, Element& src):
    Element(renderer), EventDispatcher(renderer), _evloop(evloop), _src(src)
{
    if(_evloop.render_function()) 
    {
        TSDL::safe_throw<std::runtime_error>("Could not bind render adapter to an eventloop that already have render function");
        return;
    }
    _evloop.render_function(std::bind(_render, this));
    _evloop.add_event_handler(
        SDL_WINDOWEVENT, std::bind(_handle_window_event, this, _1)
    );
    _evloop.add_event_handler(
        SDL_KEYDOWN, std::bind(_handle_keydown_event, this, _1)
    );
    _evloop.add_event_handler(
        SDL_KEYUP, std::bind(_handle_keyup_event, this, _1)
    );
    _evloop.add_event_handler(
        SDL_TEXTINPUT, std::bind(_handle_textinput_event, this, _1)
    );
    _evloop.add_event_handler(
        SDL_MOUSEMOTION, std::bind(_handle_mousemotion_event, this, _1)
    );
    _evloop.add_event_handler(
        SDL_MOUSEBUTTONDOWN, std::bind(_handle_mousebuttondown_event, this, _1)
    );
    _evloop.add_event_handler(
        SDL_MOUSEBUTTONUP, std::bind(_handle_mousebuttonup_event, this, _1)
    );
}

TSDL::elements::EventloopAdapter::~EventloopAdapter()
{
    _evloop.render_function(nullptr);
    _evloop.remove_event_handler(SDL_WINDOWEVENT);
    _evloop.remove_event_handler(SDL_KEYDOWN);
    _evloop.remove_event_handler(SDL_KEYUP);
    _evloop.remove_event_handler(SDL_TEXTINPUT);
    _evloop.remove_event_handler(SDL_MOUSEMOTION);
    _evloop.remove_event_handler(SDL_MOUSEBUTTONDOWN);
    _evloop.remove_event_handler(SDL_MOUSEBUTTONUP);
}

void TSDL::elements::EventloopAdapter::render()
{
    _src.render();
}

TSDL::TSDL_Texture& TSDL::elements::EventloopAdapter::get_texture()
{
    return _src.get_texture();
}

TSDL::elements::Element& TSDL::elements::EventloopAdapter::src() const
{
    return _src;
}