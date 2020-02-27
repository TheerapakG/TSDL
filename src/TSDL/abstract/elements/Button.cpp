#include "TSDL/abstract/elements/Button.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

#include <algorithm>
#include <cmath>

namespace
{
    using namespace ::TSDL;
    using namespace ::TSDL::events;
    using namespace ::TSDL::elements;
}

Button::Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& _size):
    sized<eventdispatcher<DependentElement>>(evloop, renderer, _size)
{
    Element::add_event_handler(
        EventType::MouseIn,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            if(state != ButtonState::CLICKED)
            {
                state = ButtonState::HOVER;
                update(); 
            }
            return true;
        }
    );

    Element::add_event_handler(
        EventType::MouseOut,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            if(state != ButtonState::CLICKED)
            {
                state = ButtonState::NORMAL;
                update();  
            }
            return true;
        }
    );

    Element::add_event_handler(
        EventType::LeftDown,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            state = ButtonState::CLICKED;
            update();
            return true;
        }
    );

    Element::add_event_handler(
        EventType::LeftUp_Inside,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            state = ButtonState::HOVER;
            dispatch_event(Caller(*this, {0, 0}), EventType::ButtonActivated, ::TSDL::null_event);
            update();
            return true;
        }
    );

    Element::add_event_handler(
        EventType::LeftUp_Outside,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            state = ButtonState::NORMAL;
            update();
            return true;
        }
    );
}

Button::Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& _size, const ListenerMap& listeners):
    sized<eventdispatcher<DependentElement>>(evloop, renderer, _size, listeners)
{
    Element::add_event_handler(
        EventType::MouseIn,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            state = ButtonState::HOVER;
            update();
            return true;
        }
    );

    Element::add_event_handler(
        EventType::MouseOut,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            state = ButtonState::NORMAL;
            update();
            return true;
        }
    );
}

Button::Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& _size, ListenerMap&& listeners):
    sized<eventdispatcher<DependentElement>>(evloop, renderer, _size, listeners)
{
    Element::add_event_handler(
        EventType::MouseIn,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            state = ButtonState::HOVER;
            update();
            return true;
        }
    );

    Element::add_event_handler(
        EventType::MouseOut,
        [this](const Caller&, const SDL_Event&) -> bool
        {
            state = ButtonState::NORMAL;
            update();
            return true;
        }
    );
}

Button& Button::normal(const std::shared_ptr<RenderSizedElement>& element)
{
    _normal = element;
    return *this;
}

Button& Button::hover(const std::shared_ptr<RenderSizedElement>& element)
{
    _hover = element;
    return *this;
}

Button& Button::clicked(const std::shared_ptr<RenderSizedElement>& element)
{
    _clicked = element;
    return *this;
}

Button& Button::front(optional_reference<sized<RenderSizedElement>> front)
{
    _front = front;
    return *this;
}

std::shared_ptr<RenderSizedElement> Button::normal()
{
    return _normal;
}

std::shared_ptr<RenderSizedElement> Button::hover()
{
    return _hover;
}

std::shared_ptr<RenderSizedElement> Button::clicked()
{
    return _clicked;
}

sized<RenderSizedElement>& Button::front()
{
    return _front.value();
}

bool Button::need_update() const
{
    switch (state)
    {
    case ButtonState::NORMAL:
        if(_normal->need_update()) return true;
        break;

    case ButtonState::HOVER:
        if(_hover->need_update()) return true;
        break;

    case ButtonState::CLICKED:
        if(_clicked->need_update()) return true;
        break;
    
    default:
        break;
    }
    return sized<eventdispatcher<DependentElement>>::need_update();
}

void Button::render(const ::TSDL::point_2d& dist)
{
    // TODO: renderer RAII
    auto& _renderer = renderer();
    switch (state)
    {
    case ButtonState::NORMAL:
        _normal->render(dist, size());
        break;

    case ButtonState::HOVER:
        _hover->render(dist, size());
        break;

    case ButtonState::CLICKED:
        _clicked->render(dist, size());
        break;
    
    default:
        break;
    }

    if(_front)
    {
        sized<RenderSizedElement>& _telement = _front.value().get();
        const ::TSDL::point_2d texture_final_size = size() - 2 * ::TSDL::point_2d(_padding, _padding);
        const ::TSDL::point_2d texture_size = _telement.size();
        double x_ratio = static_cast<double>(texture_final_size.x) / texture_size.x;
        double y_ratio = static_cast<double>(texture_final_size.y) / texture_size.y;
        double ratio = std::min(x_ratio, y_ratio);
        const ::TSDL::point_2d render_size(std::round(ratio * texture_size.x), std::round(ratio * texture_size.y));
        if(ratio>0)
        {
            _telement.render(dist + (size() - render_size)/2, render_size);
        }
    }

    not_update();
}