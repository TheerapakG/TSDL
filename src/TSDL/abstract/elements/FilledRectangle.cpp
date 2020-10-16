//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/elements/FilledRectangle.hpp"
#include "TSDL/abstract/elements/WindowAdapter.hpp"
#include "TSDL/Meta.hpp"

TSDL::elements::FilledRectangle::FilledRectangle(const FilledRectangle& other):
    attrs::sizable<RenderSizedElement>(other.size()), _color(other._color) {}

TSDL::elements::FilledRectangle::FilledRectangle(const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color):
    attrs::sizable<RenderSizedElement>(size), _color(color) {}

void TSDL::elements::FilledRectangle::render(WindowAdapter& window, const ::TSDL::point_2d& dist)
{
    render(window, dist, size());
}

void TSDL::elements::FilledRectangle::render(WindowAdapter& window, const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    window.renderer().fill_rect(
        _color,
        {dist, dist+size}
    );

    not_update();
}