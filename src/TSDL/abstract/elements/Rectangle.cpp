#include "TSDL/abstract/elements/Rectangle.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::Rectangle::Rectangle(const Rectangle& other):
    attrs::sizable<RenderSizedElement>(other.eventloop(), other.renderer(), other.size()), _color(other._color) {}

TSDL::elements::Rectangle::Rectangle(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color):
    attrs::sizable<RenderSizedElement>(evloop, renderer, size), _color(color) {}

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