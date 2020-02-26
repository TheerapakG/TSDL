#include "TSDL/abstract/elements/Rectangle.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::Rectangle::Rectangle(const Rectangle& other):
    Element(other.renderer()), sized<RenderSizedElement>(other.eventloop(), other.size()), _color(other._color) {}

TSDL::elements::Rectangle::Rectangle(EventloopAdapter& evloop, const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color):
    Element(evloop.renderer()), sized<RenderSizedElement>(evloop, size), _color(color) {}

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