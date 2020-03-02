#include "TSDL/abstract/elements/Scrollbar.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::elements
{
    int BaseHorizontalScrollbar::_bar_length() const
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
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size, const attrs::ListenerMap& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_length(content_length)
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
        EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size, attrs::ListenerMap&& listeners):
        attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>(evloop, renderer, size, listeners), 
        _content_length(content_length)
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

    point_2d BaseHorizontalScrollbar::represented_section() const
    {
        int _size_x = size().x;
        int bar_left = _bar.pos().x;
        return {(bar_left * _content_length)/_size_x, ((bar_left + _bar_length()) * _content_length)/_size_x};
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