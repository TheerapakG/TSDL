#include <optional>
#include <functional>
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

using namespace std::placeholders;

namespace
{
    using namespace TSDL;

    void _handle_window_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowShown, event
            );
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowHidden, event
            );
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowExposed, event
            );
            break;
        case SDL_WINDOWEVENT_MOVED:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowMoved, event
            );
            break;
        case SDL_WINDOWEVENT_RESIZED:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)),events::EventType::WindowResized, event
            );
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowMinimized, event
            );
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowMaximized, event
            );
            break;
        case SDL_WINDOWEVENT_RESTORED:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowRestored, event
            );
            break;
        case SDL_WINDOWEVENT_ENTER:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::MouseIn, event
            );
            break;
        case SDL_WINDOWEVENT_LEAVE:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::MouseOut, event
            );
            break;
        case SDL_WINDOWEVENT_CLOSE:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::WindowClose, event
            );
            break;
        default:
            break;
        }
    }

    void _handle_keydown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::KeyDown, event
        );
    }

    void _handle_keyup_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::KeyUp, event
        );
    }

    void _handle_textinput_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(0, 0)), events::EventType::TextInput, event
        );
    }

    void _handle_mousemotion_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
            TSDL::elements::Caller(*adapter, TSDL::point_2d(event.motion.x, event.motion.y)), events::EventType::MouseMotion, event
        );
    }

    void _handle_mousebuttondown_event(TSDL::elements::EventloopAdapter* adapter, const ::SDL_Event& event)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)),events::EventType::LeftDown, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), events::EventType::MiddleDown, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), events::EventType::RightDown, event
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
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), events::EventType::LeftUp, event
            );
            break;
        case SDL_BUTTON_MIDDLE:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), events::EventType::MiddleUp, event
            );
            break;
        case SDL_BUTTON_RIGHT:
            adapter->template src<TSDL::elements::attrs::EventDispatcher>().dispatch_event(
                TSDL::elements::Caller(*adapter, TSDL::point_2d(event.button.x, event.button.y)), events::EventType::RightUp, event
            );
            break;
        default:
            break;
        }
    }
}

TSDL::elements::EventloopAdapter::EventloopAdapter(TSDL_Eventloop& evloop): _evloop(evloop)
{
    if(_evloop.render_function()) 
    {
        TSDL::safe_throw<std::runtime_error>("Could not bind render adapter to an eventloop that already have render function");
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
            DependentElement& _c_src = src<DependentElement>();
            if (!_c_src.need_update()) return;
            _c_src.renderer().clear({ 255, 255, 255, 255 });
            _c_src.render({0, 0});
            _c_src.renderer().update();
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

void TSDL::elements::EventloopAdapter::register_not_update(TSDL::elements::DependentElement& element)
{
    _not_update_el.emplace(element);
}

void TSDL::elements::EventloopAdapter::register_call_next(std::function<void()> call)
{
    _calls.push(call);
}

void TSDL::elements::EventloopAdapter::src(std::nullopt_t)
{
    _src.reset();
}