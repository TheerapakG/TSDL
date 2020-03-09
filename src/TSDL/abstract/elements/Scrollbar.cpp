#include "TSDL/abstract/elements/Scrollbar.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::elements
{
    int BaseHorizontalScrollbar::_bar_length() const
    {
        int _size_x = size().x;
        return (_size_x*_size_x)/_content_width;
    }

    point_2d BaseHorizontalScrollbar::_bar_movement_calc(const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist)
    {
        int _ret_x = start.x + dist.x, _size_x = size().x;
        int _travel_x = _size_x - _bar_length();
        if (_ret_x < 0) { return {0, 0}; }
        if (_ret_x > _travel_x) { return {_travel_x, 0}; }
        return {_ret_x, 0};
    }

    void BaseHorizontalScrollbar::_init()
    {
        grid().add_child(_bar, {0, 0});
        // TODO: event forwarder
        _bar.add_event_handler(
            ::TSDL::events::EventType::Dragged, 
            [this](const ::TSDL::elements::Caller&, const SDL_Event& event) -> bool
            {
                return dispatch_event(Caller(*this, {0, 0}), ::TSDL::events::EventType::Dragged, event);
            }
        );
    }

    BaseHorizontalScrollbar::BaseHorizontalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_width, const point_2d& size):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size), 
        _content_width(content_width)
    {
        _init();
    }

    BaseHorizontalScrollbar::BaseHorizontalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_width, const point_2d& size, const attrs::ListenerMap& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_width(content_width)
    {
        _init();
    }

    BaseHorizontalScrollbar::BaseHorizontalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_width, const point_2d& size, attrs::ListenerMap&& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_width(content_width)
    {
        _init();
    }

    int BaseHorizontalScrollbar::content_width() const
    {
        return _content_width;
    }

    void BaseHorizontalScrollbar::content_width(int width)
    {
        _content_width = width;
        _bar.size({size().x, _bar_length()});
    }

    point_2d BaseHorizontalScrollbar::represented_section() const
    {
        int _size_x = size().x;
        int bar_left = _bar.pos().x;
        return {(bar_left * _content_width)/_size_x, ((bar_left + _bar_length()) * _content_width)/_size_x};
    }

    bool BaseHorizontalScrollbar::need_update() const
    {
        return grid().need_update();
    }

    void BaseHorizontalScrollbar::render(const ::TSDL::point_2d& dist)
    {
        grid().render(dist);
    }

    int BaseVerticalScrollbar::_bar_length() const
    {
        int _size_y = size().y;
        return (_size_y*_size_y)/_content_height;
    }

    point_2d BaseVerticalScrollbar::_bar_movement_calc(const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist)
    {
        int _ret_y = start.y + dist.y, _size_y = size().y;
        int _travel_y = _size_y - _bar_length();
        if (_ret_y < 0) { return {0, 0}; }
        if (_ret_y > _travel_y) { return {0, _travel_y}; }
        return {0, _ret_y};
    }

    void BaseVerticalScrollbar::_init()
    {
        grid().add_child(_bar, {0, 0});
        // TODO: event forwarder
        _bar.add_event_handler(
            ::TSDL::events::EventType::Dragged, 
            [this](const ::TSDL::elements::Caller&, const SDL_Event& event) -> bool
            {
                return dispatch_event(Caller(*this, {0, 0}), ::TSDL::events::EventType::Dragged, event);
            }
        );
    }

    BaseVerticalScrollbar::BaseVerticalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_height, const point_2d& size):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size), 
        _content_height(content_height)
    {
        _init();
    }

    BaseVerticalScrollbar::BaseVerticalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_height, const point_2d& size, const attrs::ListenerMap& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_height(content_height)
    {
        _init();
    }

    BaseVerticalScrollbar::BaseVerticalScrollbar(
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_height, const point_2d& size, attrs::ListenerMap&& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_height(content_height)
    {
        _init();
    }

    int BaseVerticalScrollbar::content_height() const
    {
        return _content_height;
    }
    
    void BaseVerticalScrollbar::content_height(int height)
    {
        _content_height = height;
        _bar.size({size().x, _bar_length()});
    }

    point_2d BaseVerticalScrollbar::represented_section() const
    {
        int _size_y = size().y;
        int bar_left = _bar.pos().y;
        return {(bar_left * _content_height)/_size_y, ((bar_left + _bar_length()) * _content_height)/_size_y};
    }

    bool BaseVerticalScrollbar::need_update() const
    {
        return grid().need_update();
    }

    void BaseVerticalScrollbar::render(const ::TSDL::point_2d& dist)
    {
        grid().render(dist);
    }
}