#include "TSDL/abstract/elements/Button.hpp"
#include "TSDL/TSDL_Meta.hpp"

#include <algorithm>
#include <cmath>

namespace
{
    using namespace ::TSDL::events;
}

TSDL::elements::Button::Button(TSDL_Renderer& _renderer, const point_2d& _size):
    Element(_renderer),
    sized<eventdispatcher<Element>>(std::ref(_renderer), _size)
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

TSDL::elements::Button::Button(TSDL_Renderer& _renderer, const point_2d& _size, const ListenerMap& listeners):
    Element(_renderer),
    sized<eventdispatcher<Element>>(std::ref(_renderer), _size, listeners)
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

TSDL::elements::Button::Button(TSDL_Renderer& _renderer, const point_2d& _size, ListenerMap&& listeners):
    Element(_renderer),
    sized<eventdispatcher<Element>>(std::ref(_renderer), _size, listeners)
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

TSDL::elements::Button& TSDL::elements::Button::color(const ::TSDL::color_rgba& color)
{
    _color = color;
    return *this;
}

TSDL::elements::Button& TSDL::elements::Button::hover_color(const ::TSDL::color_rgba& color)
{
    _hover_color = color;
    return *this;
}

TSDL::elements::Button& TSDL::elements::Button::clicked_color(const ::TSDL::color_rgba& color)
{
    _clicked_color = color;
    return *this;
}

TSDL::elements::Button& TSDL::elements::Button::text_color(const ::TSDL::color_rgba& color)
{
    _text_color = color;
    return *this;
}

TSDL::elements::Button& TSDL::elements::Button::texture(optional_reference<TextureElement> texture)
{
    _texture = texture;
    return *this;
}

TSDL::color_rgba TSDL::elements::Button::color()
{
    return _color;
}

TSDL::color_rgba TSDL::elements::Button::hover_color()
{
    return _hover_color;
}

TSDL::color_rgba TSDL::elements::Button::clicked_color()
{
    return _clicked_color;
}

TSDL::color_rgba TSDL::elements::Button::text_color()
{
    return _text_color;
}

TSDL::elements::TextureElement& TSDL::elements::Button::texture()
{
    return _texture.value();
}

void TSDL::elements::Button::render(const ::TSDL::point_2d& dist)
{
    // TODO: renderer RAII
    auto& _renderer = renderer();
    switch (state)
    {
    case ButtonState::NORMAL:
        _renderer.fill_rect(_color, {dist, dist+size()});
        break;

    case ButtonState::HOVER:
        _renderer.fill_rect(_hover_color, {dist, dist+size()});
        break;

    case ButtonState::CLICKED:
        _renderer.fill_rect(_clicked_color, {dist, dist+size()});
        break;
    
    default:
        break;
    }

    if(_texture)
    {
        TextureElement& _telement = _texture.value().get();
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