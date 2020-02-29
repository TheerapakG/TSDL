#include "TSDL/abstract/elements/FilledRectangle.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::FilledRectangle::FilledRectangle(const FilledRectangle& other):
    attrs::sizable<RenderSizedElement>(other.eventloop(), other.renderer(), other.size()), _color(other._color) {}

TSDL::elements::FilledRectangle::FilledRectangle(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color):
    attrs::sizable<RenderSizedElement>(evloop, renderer, size), _color(color) {}

void TSDL::elements::FilledRectangle::render(const ::TSDL::point_2d& dist)
{
    render(dist, size());
}

void TSDL::elements::FilledRectangle::render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    renderer().fill_rect(
        _color,
        {dist, dist+size}
    );

    not_update();
}