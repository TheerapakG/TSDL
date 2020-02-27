#include "TSDL/abstract/elements/FilledEllipse.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::FilledEllipse::FilledEllipse(const FilledEllipse& other):
    attrs::sizable<RenderSizedElement>(other.eventloop(), other.renderer(), other.size()), _color(other._color) {}

TSDL::elements::FilledEllipse::FilledEllipse(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ::TSDL::point_2d& size, const ::TSDL::color_rgba& color):
    attrs::sizable<RenderSizedElement>(evloop, renderer, size), _color(color) {}

void TSDL::elements::FilledEllipse::render(const ::TSDL::point_2d& dist)
{
    render(dist, size());
}

void TSDL::elements::FilledEllipse::render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    TSDL_Renderer& _render = renderer();

    color_rgba prev_col = _render.render_color();
    _render.render_color(_color); // TODO: check when noexcept signify error

    auto[x, y] = size;
    long long x2 = static_cast<long long>(x*x), y2 = static_cast<long long>(y*y);

    long long cl = 0;
    long long cr = 0;
    int d = 0;

    if(x > y)
    {
        // This algorithm approximates the center point as being in the middle of a y line
        for(int i = 0; i < (y+1)/2; i++)
        {
            while (cl < cr) {cl += y2 * (x - 2*d - 1); d++;}
            _render.draw_line(dist.x+d, dist.y + (y-1)/2-i, dist.x+x-d, dist.y + (y-1)/2-i);
            _render.draw_line(dist.x+d, dist.y + y/2+i, dist.x+x-d, dist.y + y/2+i);
            cr += x2 * (2*i + 1);
        }
    }   
    else
    {
        // This algorithm approximates the center point as being in the middle of a x line
        for(int i = 0; i < (x+1)/2; i++)
        {
            while (cl < cr) {cl += x2 * (y - 2*d - 1); d++;}
            _render.draw_line(dist.x + (x-1)/2-i, dist.y+d, dist.x + (x-1)/2-i, dist.y+y-d);
            _render.draw_line(dist.x + x/2+i, dist.y+d, dist.x + x/2+i, dist.y+y-d);
            cr += y2 * (2*i + 1);
        }
    }
    
    _render.render_color(prev_col); // TODO: check when noexcept signify error

    not_update();
}