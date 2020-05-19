//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include <optional>
#include <functional>
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/abstract/elements/WindowAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

using namespace std::placeholders;

namespace
{
    using namespace TSDL;
    using namespace TSDL::elements;

    void _handle_window_event(EventloopAdapter* adapter, const SDL_Event& event)
    {
        auto _window = adapter->windows().find(event.window.windowID);
        if (_window == adapter->windows().end()) return;
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowShown, event
            );
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowHidden, event
            );
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowExposed, event
            );
            break;
        case SDL_WINDOWEVENT_MOVED:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowMoved, event
            );
            break;
        case SDL_WINDOWEVENT_RESIZED:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowResized, event
            );
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowMinimized, event
            );
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowMaximized, event
            );
            break;
        case SDL_WINDOWEVENT_RESTORED:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowRestored, event
            );
            break;
        case SDL_WINDOWEVENT_ENTER:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::MouseIn, event
            );
            break;
        case SDL_WINDOWEVENT_LEAVE:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::MouseOut, event
            );
            break;
        case SDL_WINDOWEVENT_CLOSE:
            _window->second.get().dispatch_event(
                point_2d(0, 0), events::EventType::WindowClose, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_keydown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        auto _window = adapter->windows().find(event.key.windowID);
        if (_window == adapter->windows().end()) return;
        _window->second.get().dispatch_event(
            point_2d(0, 0), events::EventType::KeyDown, event
        );
    }

    void _handle_keyup_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        auto _window = adapter->windows().find(event.key.windowID);
        if (_window == adapter->windows().end()) return;
        _window->second.get().dispatch_event(
            point_2d(0, 0), events::EventType::KeyUp, event
        );
    }

    void _handle_textinput_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        auto _window = adapter->windows().find(event.text.windowID);
        if (_window == adapter->windows().end()) return;
        _window->second.get().dispatch_event(
            point_2d(0, 0), events::EventType::TextInput, event
        );
    }

    void _handle_mousemotion_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        auto _window = adapter->windows().find(event.motion.windowID);
        if (_window == adapter->windows().end()) return;
        _window->second.get().dispatch_event(
            point_2d(event.motion.x, event.motion.y), events::EventType::MouseMotion, event
        );
    }

    void _handle_mousebuttondown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        auto _window = adapter->windows().find(event.button.windowID);
        if (_window == adapter->windows().end()) return;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            _window->second.get().dispatch_event(
                point_2d(event.button.x, event.button.y), events::EventType::LeftDown, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            _window->second.get().dispatch_event(
                point_2d(event.button.x, event.button.y), events::EventType::MiddleDown, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            _window->second.get().dispatch_event(
                point_2d(event.button.x, event.button.y), events::EventType::RightDown, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_mousebuttonup_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        auto _window = adapter->windows().find(event.button.windowID);
        if (_window == adapter->windows().end()) return;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            _window->second.get().dispatch_event(
                point_2d(event.button.x, event.button.y), events::EventType::LeftUp, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            _window->second.get().dispatch_event(
                point_2d(event.button.x, event.button.y), events::EventType::MiddleUp, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            _window->second.get().dispatch_event(
                point_2d(event.button.x, event.button.y), events::EventType::RightUp, event
            );
            break;
        default:
            break;
        }
    }
}

namespace TSDL::elements
{
    namespace impl
    {
        optional_reference<EventloopAdapter> _current_eventloop_adapter;
    }

    EventloopAdapter::EventloopAdapter(): _evloop(current_eventloop())
    {
        if(impl::_current_eventloop_adapter.has_value())
        {
            safe_throw<std::runtime_error>("There is already an eventloop, running two eventloop simultaneously is not possible");
            return;
        }
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
                for(auto _window: _windows)
                {
                    _window.second.get().render();
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
        impl::_current_eventloop_adapter = *this;
    }

    EventloopAdapter::~EventloopAdapter()
    {
        impl::_current_eventloop_adapter.reset();
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

    void EventloopAdapter::add_window(WindowAdapter& window_adapter)
    {
        _windows.emplace(window_adapter.window().id(), window_adapter);
    }

    void EventloopAdapter::remove_window(WindowAdapter& window_adapter)
    {
        _windows.erase(window_adapter.window().id());
    }

    TSDL_Eventloop::clock::time_point EventloopAdapter::now()
    {
        return _evloop.now();
    }

    const std::unordered_map <Uint32, std::reference_wrapper<WindowAdapter>>& EventloopAdapter::windows()
    {
        return _windows;
    }

    EventloopAdapter& current_eventloop_adapter()
    {
        return impl::_current_eventloop_adapter.value();
    }
}