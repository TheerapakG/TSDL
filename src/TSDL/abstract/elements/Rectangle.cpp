#include "TSDL/abstract/elements/Rectangle.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::Rectangle::Rectangle(const Rectangle& other):
    Element(other.renderer()), sized<RenderSizedElement>(other.renderer(), other.size()), _color(other._color) {}

TSDL::elements::Rectangle::Rectangle(::TSDL::TSDL_Renderer& renderer, const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color):
    Element(renderer), sized<RenderSizedElement>(renderer, size), _color(color) {}

void TSDL::elements::Rectangle::render(const ::TSDL::point_2d& dist)
{
    render(dist, size());
}

void TSDL::elements::Rectangle::render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    renderer().fill_rect(
        _color,
        {dist, dist+size}
    );

    not_update();
}