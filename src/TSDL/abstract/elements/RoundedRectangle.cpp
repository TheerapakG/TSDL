//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/elements/RoundedRectangle.hpp"
#include "TSDL/abstract/elements/WindowAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::RoundedRectangle::RoundedRectangle(const RoundedRectangle& other):
    attrs::sizable<RenderSizedElement>(other.size()), _color(other._color), _r(other._r) {}

TSDL::elements::RoundedRectangle::RoundedRectangle(const ::TSDL::point_2d& size, int r, const ::TSDL::color_rgba& color):
    attrs::sizable<RenderSizedElement>(size), _color(color), _r(r) {}

void TSDL::elements::RoundedRectangle::render(WindowAdapter& window, const ::TSDL::point_2d& dist)
{
    render(window, dist, size());
}

void TSDL::elements::RoundedRectangle::render(WindowAdapter& window, const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    TSDL_Renderer& _render = window.renderer();

    color_rgba prev_col = _render.render_color();
    _render.render_color(_color); // TODO: check when noexcept signify error

    auto[x, y] = size;

    int cl = 0;
    int cr = 0;
    int d = 0;

    for(int i = 0; i < _r; i++)
    {
        while (cl < cr) {cl += (2*_r - 2*d - 1); d++;}
        _render.draw_line(dist.x+d, dist.y + _r-i-1, dist.x+x-d, dist.y + _r-i-1);
        _render.draw_line(dist.x+d, dist.y + y-_r+i, dist.x+x-d, dist.y + y-_r+i);
        cr += (2*i + 1);
    }

    _render.fill_rect({dist+point_2d{0, _r}, dist+size-point_2d{0, _r}});
    
    _render.render_color(prev_col); // TODO: check when noexcept signify error

    not_update();
}