//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/visuals/elements/HollowedRectangle.hpp"
#include "TSDL/visuals/elements/WindowAdapter.hpp"
#include "TSDL/Meta.hpp"

TSDL::elements::HollowedRectangle::HollowedRectangle(const HollowedRectangle& other):
    attrs::sizable<RenderSizedElement>(other.size()), _color(other._color) {}

TSDL::elements::HollowedRectangle::HollowedRectangle(const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color, int thickness):
    attrs::sizable<RenderSizedElement>(size), _color(color), _thickness(thickness) {}

void TSDL::elements::HollowedRectangle::render(WindowAdapter& window, const ::TSDL::point_2d& dist)
{
    render(window, dist, size());
}

void TSDL::elements::HollowedRectangle::render(WindowAdapter& window, const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    window.renderer().fill_rect(
        _color,
        {dist, dist+point_2d{0, _thickness}}
    );
    window.renderer().fill_rect(
        _color,
        {dist, dist+point_2d{_thickness, 0}}
    );
    window.renderer().fill_rect(
        _color,
        {dist+size-point_2d{0, _thickness}, dist+size}
    );
    window.renderer().fill_rect(
        _color,
        {dist+size-point_2d{_thickness, 0}, dist+size}
    );

    not_update();
}