#include "TSDL/abstract/elements/RoundedRectangle.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::RoundedRectangle::RoundedRectangle(const RoundedRectangle& other):
    attrs::sizable<RenderSizedElement>(other.eventloop(), other.renderer(), other.size()), _color(other._color), _r(other._r) {}

TSDL::elements::RoundedRectangle::RoundedRectangle(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ::TSDL::point_2d& size, int r, const ::TSDL::color_rgba& color):
    attrs::sizable<RenderSizedElement>(evloop, renderer, size), _color(color), _r(r) {}

void TSDL::elements::RoundedRectangle::render(const ::TSDL::point_2d& dist)
{
    render(dist, size());
}

void TSDL::elements::RoundedRectangle::render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    TSDL_Renderer& _render = renderer();

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

    renderer().fill_rect({dist+point_2d{0, _r}, dist+size-point_2d{0, _r}});
    
    _render.render_color(prev_col); // TODO: check when noexcept signify error

    not_update();
}