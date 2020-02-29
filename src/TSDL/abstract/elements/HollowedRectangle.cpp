#include "TSDL/abstract/elements/HollowedRectangle.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::HollowedRectangle::HollowedRectangle(const HollowedRectangle& other):
    attrs::sizable<RenderSizedElement>(other.eventloop(), other.renderer(), other.size()), _color(other._color) {}

TSDL::elements::HollowedRectangle::HollowedRectangle(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color, int thickness):
    attrs::sizable<RenderSizedElement>(evloop, renderer, size), _color(color), _thickness(thickness) {}

void TSDL::elements::HollowedRectangle::render(const ::TSDL::point_2d& dist)
{
    render(dist, size());
}

void TSDL::elements::HollowedRectangle::render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    renderer().fill_rect(
        _color,
        {dist, dist+point_2d{0, _thickness}}
    );
    renderer().fill_rect(
        _color,
        {dist, dist+point_2d{_thickness, 0}}
    );
    renderer().fill_rect(
        _color,
        {dist+size-point_2d{0, _thickness}, dist+size}
    );
    renderer().fill_rect(
        _color,
        {dist+size-point_2d{_thickness, 0}, dist+size}
    );

    not_update();
}