#include <optional>
#include <functional>
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

using namespace std::placeholders;

namespace
{
    using namespace TSDL;
    using namespace TSDL::elements;

    void _handle_window_event(EventloopAdapter* adapter, const SDL_Event& event)
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowShown, event
            );
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowHidden, event
            );
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowExposed, event
            );
            break;
        case SDL_WINDOWEVENT_MOVED:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowMoved, event
            );
            break;
        case SDL_WINDOWEVENT_RESIZED:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowResized, event
            );
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowMinimized, event
            );
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowMaximized, event
            );
            break;
        case SDL_WINDOWEVENT_RESTORED:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowRestored, event
            );
            break;
        case SDL_WINDOWEVENT_ENTER:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::MouseIn, event
            );
            break;
        case SDL_WINDOWEVENT_LEAVE:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::MouseOut, event
            );
            break;
        case SDL_WINDOWEVENT_CLOSE:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(0, 0)), events::EventType::WindowClose, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_keydown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src<attrs::EventLookupable>().dispatch_event(
            Caller(*adapter, point_2d(0, 0)), events::EventType::KeyDown, event
        );
    }

    void _handle_keyup_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src<attrs::EventLookupable>().dispatch_event(
            Caller(*adapter, point_2d(0, 0)), events::EventType::KeyUp, event
        );
    }

    void _handle_textinput_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src<attrs::EventLookupable>().dispatch_event(
            Caller(*adapter, point_2d(0, 0)), events::EventType::TextInput, event
        );
    }

    void _handle_mousemotion_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->src<attrs::EventLookupable>().dispatch_event(
            Caller(*adapter, point_2d(event.motion.x, event.motion.y)), events::EventType::MouseMotion, event
        );
    }

    void _handle_mousebuttondown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(event.button.x, event.button.y)), events::EventType::LeftDown, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(event.button.x, event.button.y)), events::EventType::MiddleDown, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(event.button.x, event.button.y)), events::EventType::RightDown, event
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
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(event.button.x, event.button.y)), events::EventType::LeftUp, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(event.button.x, event.button.y)), events::EventType::MiddleUp, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter->src<attrs::EventLookupable>().dispatch_event(
                Caller(*adapter, point_2d(event.button.x, event.button.y)), events::EventType::RightUp, event
            );
            break;
        default:
            break;
        }
    }
}

namespace TSDL::elements
{
    EventloopAdapter::EventloopAdapter(TSDL_Eventloop& evloop): _evloop(evloop)
    {
        if(_evloop.render_function()) 
        {
            safe_throw<std::runtime_error>("Could not bind render adapter to an eventloop that already have render function");
            return;
        }
        _evloop.render_function(
            [this]() -> void
            {
                while(!_calls.empty())
                {
                    _calls.front()();
                    _calls.pop();
                }
                if (_d_src.has_value())
                {
                    DependentElement& d_src = _d_src.value().get();
                    if (!d_src.need_update()) return;
                    d_src.renderer().clear({255, 255, 255, 255});
                    d_src.render({0, 0});
                    d_src.renderer().update();
                }
                while(!_not_update_el.empty())
                {
                    _not_update_el.front().get()._update = false;
                    _not_update_el.pop();
                }
            }
        );
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

    EventloopAdapter::~EventloopAdapter()
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

    void EventloopAdapter::register_not_update(DependentElement& element)
    {
        _not_update_el.emplace(element);
    }

    void EventloopAdapter::register_call_next(std::function<void()> call)
    {
        _calls.push(call);
    }

    void EventloopAdapter::src(std::nullopt_t)
    {
        _src.reset();
        _d_src.reset();
    }
}