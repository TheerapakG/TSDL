#include "TSDL/abstract/elements/Scrollbar.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::elements
{
    int BaseHorizontalScrollbar::_bar_length()
    {
        int _size_x = size().x;
        return (_size_x*_size_x)/_content_length;
    }

    point_2d BaseHorizontalScrollbar::_bar_movement_calc(const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist)
    {
        int _ret_x = start.x + dist.x, _size_x = size().x;
        int _travel_x = _size_x - _bar_length();
        if (_ret_x < 0) { return {0, 0}; }
        if (_ret_x > _travel_x) { return {_travel_x, 0}; }
        return {_ret_x, 0};
    }

    BaseHorizontalScrollbar::BaseHorizontalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size), 
        _content_length(content_length)
    {
        grid().add_child(_bar, {0, 0});
    }

    BaseHorizontalScrollbar::BaseHorizontalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size, const attrs::ListenerMap& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_length(content_length)
    {
        grid().add_child(_bar, {0, 0});
    }

    BaseHorizontalScrollbar::BaseHorizontalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size, attrs::ListenerMap&& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_length(content_length)
    {
        grid().add_child(_bar, {0, 0});
    }

    bool BaseHorizontalScrollbar::need_update() const
    {
        return grid().need_update();
    }

    void BaseHorizontalScrollbar::render(const ::TSDL::point_2d& dist)
    {
        grid().render(dist);
    }
}