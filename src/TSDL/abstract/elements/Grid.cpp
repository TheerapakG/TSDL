#include "TSDL/abstract/elements/Grid.hpp"
#include "TSDL/abstract/elements/Scrollbar.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::elements
{
    Grid::Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer): _Grid<Grid>(evloop, renderer) {}
    
    Grid::Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, const attrs::ListenerMap& listeners): 
        _Grid<Grid>(evloop, renderer, listeners) {}

    Grid::Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, attrs::ListenerMap&& listeners): 
        _Grid<Grid>(evloop, renderer, listeners) {}

    void GridWithScrollbar::_init()
    {
        hbar = new BaseHorizontalScrollbar(eventloop(), renderer(), size().x, {size().x - _bar_width, _bar_width});
        vbar = new BaseVerticalScrollbar(eventloop(), renderer(), size().y, {_bar_width, size().y - _bar_width});
        hbar->dispatch_event_direct(events::EventType::Dragged, *this);
        vbar->dispatch_event_direct(events::EventType::Dragged, *this);
        add_child(*hbar, {0, size().y - _bar_width});
        add_child(*vbar, {size().x - _bar_width, 0});
        grid().render_position({-_margin, size()-_margin});
    }

    GridWithScrollbar::GridWithScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, int bar_width, const point_2d& margin)
        : attrs::sizable<_Grid<GridWithScrollbar>>(evloop, renderer, size), _underly(evloop, renderer), _bar_width(bar_width), _margin(margin)
    {
        _init();
    }
    GridWithScrollbar::GridWithScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, int bar_width, const attrs::ListenerMap& listeners, const point_2d& margin)
        : attrs::sizable<_Grid<GridWithScrollbar>>(evloop, renderer, size), _underly(evloop, renderer, listeners), _bar_width(bar_width), _margin(margin)
    {
        _init();
    }
    GridWithScrollbar::GridWithScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, int bar_width, attrs::ListenerMap&& listeners, const point_2d& margin)
        : attrs::sizable<_Grid<GridWithScrollbar>>(evloop, renderer, size), _underly(evloop, renderer, listeners), _bar_width(bar_width), _margin(margin)
    {
        _init();
    }
    GridWithScrollbar::~GridWithScrollbar()
    {
        delete hbar;
        delete vbar;
    }

    template <>
    bool GridWithScrollbar::dispatch_templated_event<events::EventType::Dragged>(const Caller&, const SDL_Event&)
    {
        point_2d region_x = hbar->represented_section();
        point_2d region_y = vbar->represented_section();
        point_2d lowerbound = grid().bound().topleft();
        grid().render_position(
            {
                region_x.x + lowerbound.x - _margin.x, 
                region_y.x + lowerbound.y - _margin.y,
                region_x.y + lowerbound.x - _margin.x,
                region_y.y + lowerbound.y - _margin.y
            }
        );
        return true;
    }

    Grid& GridWithScrollbar::grid()
    {
        return _underly;
    }

    const Grid& GridWithScrollbar::grid() const
    {
        return _underly;
    }

    bool GridWithScrollbar::need_update() const
    {
        return grid().need_update() || attrs::sizable<impl::_Grid<GridWithScrollbar>>::need_update();
    }

    /*
    Re-render this element
    */
    void GridWithScrollbar::render(const ::TSDL::point_2d& dist)
    {
        point_2d _grid_size = grid().size();
        hbar->content_width(_grid_size.x + 2*_margin.x);
        vbar->content_height(_grid_size.y + 2 * _margin.y);
        grid().render(dist);
        attrs::sizable<impl::_Grid<GridWithScrollbar>>::render(dist);
    }
}