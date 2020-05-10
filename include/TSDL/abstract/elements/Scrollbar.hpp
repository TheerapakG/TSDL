//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_SCROLLBAR_
#define TSDL_ELEMENTS_SCROLLBAR_

#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/abstract/elements/attrs/Gridded.hpp"
#include "TSDL/abstract/elements/attrs/Dragable.hpp"
#include "TSDL/abstract/elements/attrs/EventForwarder.hpp"
#include "TSDL/abstract/elements/Grid.hpp"
#include "TSDL/abstract/elements/Button.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

#include "TSDL/TSDL_Meta.hpp"
#include <memory>

namespace TSDL::elements
{
    namespace impl
    {
        using namespace std::placeholders;

        template <typename Scrollbar_T>
        class _Bar: public attrs::dragable<_Button<_Bar<Scrollbar_T>>>
        {
            private:
                Scrollbar_T* _scrollbar_ptr;

            public:
            template <typename ...Args>
            _Bar(attrs::DragablePosTransformer pos_transform, const point_2d& size, Scrollbar_T* scrollbar):
                attrs::dragable<_Button<_Bar>>(pos_transform, size), _scrollbar_ptr(scrollbar) {}

            template <events::EventType eventtype>
            bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
            {
                return attrs::dragable<_Button<_Bar<Scrollbar_T>>>::dispatch_templated_event<eventtype>(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::Dragged>(const Caller& caller, const SDL_Event& event)
            {
                return _scrollbar_ptr->dispatch_event(Caller{*_scrollbar_ptr, {0, 0}}, events::EventType::Dragged, event);
            }
        };

        template <typename Derived>
        using _BaseScrollbar_Attrs = attrs::gridded<attrs::sizable<attrs::eventdispatcher<attrs::staticeventlookup<DependentElement, Derived>>>, Grid>;

        template <typename Derived>
        class _BaseHorizontalScrollbar: public _BaseScrollbar_Attrs<Derived>
        {
            int _content_width;

            virtual int _bar_length() const            
            {
                int _size_x = size().x;
                return std::min((_size_x*_size_x)/_content_width, _content_width);
            }

            point_2d _bar_movement_calc(const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist)
            {
                int _ret_x = start.x + dist.x, _size_x = size().x;
                int _travel_x = _size_x - _bar_length();
                if (_ret_x < 0) { return {0, 0}; }
                if (_ret_x > _travel_x) { return {_travel_x, 0}; }
                return {_ret_x, 0};
            }

            _Bar<_BaseHorizontalScrollbar<Derived>> _bar{
                std::bind(std::mem_fn(&_BaseHorizontalScrollbar::_bar_movement_calc), this, _1, _2),
                point_2d{_bar_length(), size().y},
                this
            };

            void _init()
            {
                grid().add_child(_bar, {0, 0});
            }

            public:
            _BaseHorizontalScrollbar(int content_width, const point_2d& size):
                _BaseScrollbar_Attrs<Derived>(size), 
                _content_width(content_width)
            {
                _init();
            }
            _BaseHorizontalScrollbar(int content_width, const point_2d& size, const attrs::ListenerMap& listeners):
                _BaseScrollbar_Attrs<Derived>(size, listeners), 
                _content_width(content_width)
            {
                _init();
            }
            _BaseHorizontalScrollbar(int content_width, const point_2d& size, attrs::ListenerMap&& listeners):
                _BaseScrollbar_Attrs<Derived>(size, listeners), 
                _content_width(content_width)
            {
                _init();
            }

            template <events::EventType eventtype>
            bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
            {
                return grid().dispatch_templated_event<eventtype>(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::Dragged>(const Caller& caller, const SDL_Event& event)
            {
                return _BaseScrollbar_Attrs<Derived>::dispatch_templated_event<events::EventType::Dragged>(Caller{*this, {0, 0}}, event);
            }

            int content_width() const
            {
                return _content_width;
            }
            void content_width(int width)
            {
                _content_width = width;
                _bar.size({size().x, _bar_length()});
            }

            point_2d represented_section() const
            {
                int _size_x = size().x;
                int bar_left = _bar.pos().x;
                return {(bar_left * _content_width)/_size_x, ((bar_left + _bar_length()) * _content_width)/_size_x};
            }
        };

        template <typename Derived>
        class _BaseVerticalScrollbar: public _BaseScrollbar_Attrs<Derived>
        {
            int _content_height;

            virtual int _bar_length() const
            {
                int _size_y = size().y;
                return std::min((_size_y*_size_y)/_content_height, _content_height);
            }

            point_2d _bar_movement_calc(const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist)
            {
                int _ret_y = start.y + dist.y, _size_y = size().y;
                int _travel_y = _size_y - _bar_length();
                if (_ret_y < 0) { return {0, 0}; }
                if (_ret_y > _travel_y) { return {0, _travel_y}; }
                return {0, _ret_y};
            }

            _Bar<_BaseVerticalScrollbar<Derived>> _bar{
                std::bind(std::mem_fn(&_BaseVerticalScrollbar::_bar_movement_calc), this, _1, _2),
                point_2d{size().x, _bar_length()},
                this
            };

            void _init()
            {
                grid().add_child(_bar, {0, 0});
            }

            public:
            _BaseVerticalScrollbar(int content_height, const point_2d& size):
                _BaseScrollbar_Attrs<Derived>(size), 
                _content_height(content_height)
            {
                _init();
            }
            _BaseVerticalScrollbar(int content_height, const point_2d& size, const attrs::ListenerMap& listeners):
                _BaseScrollbar_Attrs<Derived>(size, listeners), 
                _content_height(content_height)
            {
                _init();
            }
            _BaseVerticalScrollbar(Derived* d_ptr, int content_height, const point_2d& size, attrs::ListenerMap&& listeners):
                _BaseScrollbar_Attrs<Derived>(d_ptr, size, listeners), 
                _content_height(content_height)
            {
                _init();
            }

            template <events::EventType eventtype>
            bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
            {
                return grid().dispatch_templated_event<eventtype>(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::Dragged>(const Caller& caller, const SDL_Event& event)
            {
                return _BaseScrollbar_Attrs<Derived>::dispatch_templated_event<events::EventType::Dragged>(Caller{*this, {0, 0}}, event);
            }

            int content_height() const            
            {
                return _content_height;
            }
            void content_height(int height)            
            {
                _content_height = height;
                _bar.size({size().x, _bar_length()});
            }

            point_2d represented_section() const
            {
                int _size_y = size().y;
                int bar_left = _bar.pos().y;
                return {(bar_left * _content_height)/_size_y, ((bar_left + _bar_length()) * _content_height)/_size_y};
            }
        };
    }
    
    class BaseHorizontalScrollbar: public impl::_BaseHorizontalScrollbar<BaseHorizontalScrollbar>
    {
        public:        
        BaseHorizontalScrollbar(int content_width, const point_2d& size);
        BaseHorizontalScrollbar(int content_width, const point_2d& size, const attrs::ListenerMap& listeners);
        BaseHorizontalScrollbar(int content_width, const point_2d& size, attrs::ListenerMap&& listeners);
    };

    class BaseVerticalScrollbar: public impl::_BaseVerticalScrollbar<BaseVerticalScrollbar>
    {
        public:        
        BaseVerticalScrollbar(int content_height, const point_2d& size);
        BaseVerticalScrollbar(int content_height, const point_2d& size, const attrs::ListenerMap& listeners);
        BaseVerticalScrollbar(int content_height, const point_2d& size, attrs::ListenerMap&& listeners);
    };
}

#endif