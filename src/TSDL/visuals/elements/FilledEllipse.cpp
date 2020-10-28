//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/visuals/elements/FilledEllipse.hpp"
#include "TSDL/visuals/elements/WindowAdapter.hpp"
#include "TSDL/Meta.hpp"

TSDL::elements::FilledEllipse::FilledEllipse(const FilledEllipse& other):
    attrs::sizable<RenderSizedElement>(other.size()), _color(other._color) {}

TSDL::elements::FilledEllipse::FilledEllipse(const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color):
    attrs::sizable<RenderSizedElement>(size), _color(color) {}

void TSDL::elements::FilledEllipse::render(WindowAdapter& window, const ::TSDL::point_2d& dist) const
{
    render(window, dist, size());
}

void TSDL::elements::FilledEllipse::render(WindowAdapter& window, const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size) const
{
    Renderer& _render = window.renderer();

    color_rgba prev_col = _render.render_color();
    _render.render_color(_color); // TODO: check when noexcept signify error

    auto[x, y] = size;
    long long _x = x, _y = y;
    long long x2 = _x*_x, y2 = _y*_y;

    long long cl = 0;
    long long cr = 0;
    int d = 0;

    if(x > y)
    {
        // This algorithm approximates the center point as being in the middle of a y line
        for(int i = 0; i < (y+1)/2; i++)
        {
            while (cl < cr) {cl += y2 * (_x - d - d - 1); d++;}
            _render.draw_line(dist.x+d, dist.y + (y-1)/2-i, dist.x+x-d, dist.y + (y-1)/2-i);
            _render.draw_line(dist.x+d, dist.y + y/2+i, dist.x+x-d, dist.y + y/2+i);
            cr += x2 * i * 2 + x2;
        }
    }   
    else
    {
        // This algorithm approximates the center point as being in the middle of a x line
        for(int i = 0; i < (x+1)/2; i++)
        {
            while (cl < cr) {cl += x2 * (_y - d - d - 1); d++;}
            _render.draw_line(dist.x + (x-1)/2-i, dist.y+d, dist.x + (x-1)/2-i, dist.y+y-d);
            _render.draw_line(dist.x + x/2+i, dist.y+d, dist.x + x/2+i, dist.y+y-d);
            cr += y2 * i * 2 + y2;
        }
    }
    
    _render.render_color(prev_col); // TODO: check when noexcept signify error

    not_update();
}