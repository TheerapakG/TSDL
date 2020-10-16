//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_GRID_
#define TSDL_ELEMENTS_GRID_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include <limits>
#include "TSDL/abstract/elements/ElementHolder.hpp"
#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/Meta.hpp"

namespace TSDL::elements
{
    namespace impl
    {
        template <typename Derived>
        using _Grid_Attrs = attrs::eventdispatcher<attrs::staticeventlookup<ElementHolder, Derived>>;

        template <typename Derived>
        class _Grid: public _Grid_Attrs<Derived>
        {
            public:
            using Subelement_vector = typename ElementHolder::Subelement_vector;

            private:
            std::optional<Element_Traits> _current_mouse_focus;
            std::optional<Element_Traits> _left_origin, _right_origin, _middle_origin;
            point_2d _mouse_location = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
            SDL_Event _last_mousemotion_event = ::TSDL::null_event;

            public:
            _Grid(): _Grid_Attrs<Derived>() {}
            _Grid(const attrs::ListenerMap& listeners): _Grid_Attrs<Derived>(listeners) {}
            _Grid(attrs::ListenerMap&& listeners): _Grid_Attrs<Derived>(listeners) {}

            template <events::EventType eventtype>
            bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
            {
                return _Grid_Attrs<Derived>::dispatch_templated_event<eventtype>(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::MouseMotion>(const Caller& caller, const SDL_Event& event)
            {
                point_2d _dist = caller.event_location + render_position().topleft();
                _mouse_location = _dist;
                _last_mousemotion_event = event;

                if(_left_origin.has_value())
                {
                    attrs::EventLookupable* _actual_origin_e = _left_origin.value();
                    DependentElement* _actual_origin = _left_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(
                        Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, 
                        events::EventType::MouseMotion, event
                    );
                    return false;
                }
                if(_middle_origin.has_value())
                {
                    attrs::EventLookupable* _actual_origin_e = _middle_origin.value();
                    DependentElement* _actual_origin = _middle_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(
                        Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, 
                        events::EventType::MouseMotion, event
                    );
                    return false;
                }
                if(_right_origin.has_value())
                {
                    attrs::EventLookupable* _actual_origin_e = _right_origin.value();
                    DependentElement* _actual_origin = _right_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(
                        Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, 
                        events::EventType::MouseMotion, event
                    );
                    return false;
                }

                for(const Subelement& subelement: ::TSDL::reverse(get_child_order()))
                {
                    auto& [element_ptr, dim] = subelement;
                    auto [topleft, bottomright] = dim;

                    attrs::Sized* _sized = element_ptr;
                    if(_sized != nullptr) bottomright = topleft + _sized->size();

                    attrs::EventLookupable* element_e = element_ptr;
                    DependentElement* element = element_ptr;

                    if (topleft.x < _dist.x && _dist.x < bottomright.x &&
                        topleft.y < _dist.y && _dist.y < bottomright.y)
                    {
                        if(!_current_mouse_focus.has_value())
                        {
                            _current_mouse_focus = element_ptr;
                            if (element_e != nullptr) element_e->dispatch_event(Caller{*this, _dist - topleft}, events::EventType::MouseIn, event);
                            if (element_e != nullptr) element_e->dispatch_event(Caller{*this, _dist - topleft}, events::EventType::MouseMotion, event);
                        }
                        else if(*element == *static_cast<DependentElement*>(_current_mouse_focus.value()))
                        {
                            if (element_e != nullptr) element_e->dispatch_event(Caller{*this, _dist - topleft}, events::EventType::MouseMotion, event);
                        }
                        else
                        {
                            attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                            DependentElement* _actual_focus = _current_mouse_focus.value();
                            if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::MouseOut, event);
                            _current_mouse_focus = element_ptr;
                            if (element_e != nullptr) element_e->dispatch_event(Caller{*this, _dist - topleft}, events::EventType::MouseIn, event);
                        }
                        return false;
                    }
                }

                if(_current_mouse_focus.has_value())
                {
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::MouseOut, event);
                    _current_mouse_focus.reset();
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::MouseOut>(const Caller& caller, const SDL_Event& event)
            {
                point_2d _dist = caller.event_location + render_position().topleft();

                if (_current_mouse_focus.has_value())
                {
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::MouseOut, event);
                    _current_mouse_focus.reset();
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::LeftDown>(const Caller& caller, const SDL_Event& event)
            {
                if(_current_mouse_focus.has_value())
                {
                    _left_origin = _current_mouse_focus;
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::LeftDown, event);
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::LeftUp>(const Caller& caller, const SDL_Event& event)
            {
                if(_current_mouse_focus.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::LeftUp, event);
                    if(_left_origin.has_value())
                    {
                        attrs::EventLookupable* _actual_origin_e = _left_origin.value();
                        DependentElement* _actual_origin = _left_origin.value();
                        if(_actual_origin == _actual_focus)
                        {
                            if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::LeftUp_Inside, event);
                        }
                        else
                        {
                            if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::LeftUp_Outside, event);
                        }
                        _left_origin.reset();
                    }
                }
                else if(_left_origin.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_origin_e = _left_origin.value();
                    DependentElement* _actual_origin = _left_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::LeftUp_Outside, event);
                    _left_origin.reset();
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::LeftUp_Outside>(const Caller& caller, const SDL_Event& event)
            { 
                if(_left_origin.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_origin_e = _left_origin.value();
                    DependentElement* _actual_origin = _left_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::LeftUp_Outside, event);
                    _left_origin.reset();
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::RightDown>(const Caller& caller, const SDL_Event& event)
            {
                if(_current_mouse_focus.has_value())
                {
                    _right_origin = _current_mouse_focus;
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::RightDown, event);
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::RightUp>(const Caller& caller, const SDL_Event& event)
            {
                if(_current_mouse_focus.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::RightUp, event);
                    if(_right_origin.has_value())
                    {
                        attrs::EventLookupable* _actual_origin_e = _right_origin.value();
                        DependentElement* _actual_origin = _right_origin.value();
                        if(_actual_origin == _actual_focus)
                        {
                            if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::RightUp_Inside, event);
                        }
                        else
                        {
                            if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::RightUp_Outside, event);
                        }
                        _right_origin.reset();
                    }
                }
                else if(_right_origin.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_origin_e = _right_origin.value();
                    DependentElement* _actual_origin = _right_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::RightUp_Outside, event);
                    _right_origin.reset();
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::RightUp_Outside>(const Caller& caller, const SDL_Event& event)
            { 
                if(_right_origin.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_origin_e = _right_origin.value();
                    DependentElement* _actual_origin = _right_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::RightUp_Outside, event);
                    _right_origin.reset();
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::MiddleDown>(const Caller& caller, const SDL_Event& event)
            {
                if(_current_mouse_focus.has_value())
                {
                    _middle_origin = _current_mouse_focus;
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::MiddleDown, event);
                }
                return false;
            }
            
            template <>
            bool dispatch_templated_event<events::EventType::MiddleUp>(const Caller& caller, const SDL_Event& event)
            {
                if(_current_mouse_focus.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_focus_e = _current_mouse_focus.value();
                    DependentElement* _actual_focus = _current_mouse_focus.value();
                    if (_actual_focus_e != nullptr) _actual_focus_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_focus).dimension.first}, events::EventType::MiddleUp, event);
                    if(_middle_origin.has_value())
                    {
                        attrs::EventLookupable* _actual_origin_e = _middle_origin.value();
                        DependentElement* _actual_origin = _middle_origin.value();
                        if(_actual_origin == _actual_focus)
                        {
                            if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::MiddleUp_Inside, event);
                        }
                        else
                        {
                            if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::MiddleUp_Outside, event);
                        }
                        _middle_origin.reset();
                    }
                }
                else if(_middle_origin.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_origin_e = _middle_origin.value();
                    DependentElement* _actual_origin = _middle_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::MiddleUp_Outside, event);
                    _middle_origin.reset();
                }
                return false;
            }

            template <>
            bool dispatch_templated_event<events::EventType::MiddleUp_Outside>(const Caller& caller, const SDL_Event& event)
            { 
                if(_middle_origin.has_value())
                {
                    point_2d _dist = caller.event_location + render_position().topleft();
                    attrs::EventLookupable* _actual_origin_e = _middle_origin.value();
                    DependentElement* _actual_origin = _middle_origin.value();
                    if (_actual_origin_e != nullptr) _actual_origin_e->dispatch_event(Caller{*this, _dist - child_info(*_actual_origin).dimension.first}, events::EventType::MiddleUp_Outside, event);
                    _middle_origin.reset();
                }
                return false;
            }

            using _Grid_Attrs<Derived>::add_child;
            virtual void add_child(const Subelement& formed_subelement) override
            {
                _Grid_Attrs<Derived>::add_child(formed_subelement);
                if (_left_origin.has_value()||_middle_origin.has_value()||_right_origin.has_value()) return;

                auto& [element, dim] = formed_subelement;
                auto [topleft, bottomright] = dim;
                
                attrs::Sized* _sized = element;
                if (_sized != nullptr) bottomright = topleft + _sized->size();

                attrs::EventLookupable* element_ptr = element;

                if (topleft.x < _mouse_location.x && _mouse_location.x < bottomright.x &&
                    topleft.y < _mouse_location.y && _mouse_location.y < bottomright.y)
                {
                    _current_mouse_focus = element;
                    if (element_ptr != nullptr) element_ptr->dispatch_event(
                        Caller{*this, _mouse_location - topleft},
                        events::EventType::MouseIn, _last_mousemotion_event
                    );
                }
            }
            virtual typename Subelement_vector::iterator add_child(const Subelement& formed_subelement, int order) override
            {
                auto it = _Grid_Attrs<Derived>::add_child(formed_subelement, order);
                if(std::next(it)!=get_child_order().end()) return it;
                if(_left_origin.has_value()||_middle_origin.has_value()||_right_origin.has_value()) return it;

                auto& [element, dim] = formed_subelement;
                auto [topleft, bottomright] = dim;
                
                attrs::Sized* _sized = element;
                if (_sized != nullptr) bottomright = topleft + _sized->size();

                attrs::EventLookupable* element_ptr = element;

                if (topleft.x < _mouse_location.x && _mouse_location.x < bottomright.x &&
                    topleft.y < _mouse_location.y && _mouse_location.y < bottomright.y)
                {
                    _current_mouse_focus = element;
                    if (element_ptr != nullptr) element_ptr->dispatch_event(
                        Caller{*this, _mouse_location - topleft},
                        events::EventType::MouseIn, _last_mousemotion_event
                    );
                }

                return it;
            }

            virtual typename Subelement_vector::iterator reorder_child(DependentElement& subelement, int order) override
            {
                auto it = _Grid_Attrs<Derived>::reorder_child(subelement, order);
                if(std::next(it)!=get_child_order().end()) return it;
                if(_left_origin.has_value()||_middle_origin.has_value()||_right_origin.has_value()) return it;

                auto& [element, dim] = *it;
                auto [topleft, bottomright] = dim;
                
                attrs::Sized* _sized = element;
                if (_sized != nullptr) bottomright = topleft + _sized->size();

                attrs::EventLookupable* element_ptr = element;

                if (topleft.x < _mouse_location.x && _mouse_location.x < bottomright.x &&
                    topleft.y < _mouse_location.y && _mouse_location.y < bottomright.y)
                {
                    _current_mouse_focus = element;
                    if (element_ptr != nullptr) element_ptr->dispatch_event(
                        Caller{*this, _mouse_location - topleft},
                        events::EventType::MouseIn, _last_mousemotion_event
                    );
                }

                return it;
            }

            virtual void remove_child(DependentElement& element) override
            {
                if(_current_mouse_focus.has_value())
                {
                    if(*static_cast<Element*>(_current_mouse_focus.value())==element) _current_mouse_focus.reset();
                }
                if(_left_origin.has_value())
                {
                    if(*static_cast<Element*>(_left_origin.value())==element) _left_origin.reset();
                }
                if(_middle_origin.has_value())
                {
                    if(*static_cast<Element*>(_middle_origin.value())==element) _middle_origin.reset();
                }
                if(_right_origin.has_value())
                {
                    if(*static_cast<Element*>(_right_origin.value())==element) _right_origin.reset();
                }
                _Grid_Attrs<Derived>::remove_child(element);
            }
        };
    }

    class Grid: public impl::_Grid<Grid>
    {
        public:
        Grid();
        Grid(const attrs::ListenerMap& listeners);
        Grid(attrs::ListenerMap&& listeners);
    };

    class BaseHorizontalScrollbar;
    class BaseVerticalScrollbar;

    // TODO: really change size
    class GridWithScrollbar: public attrs::sizable<impl::_Grid<GridWithScrollbar>>
    {
        private:
        BaseHorizontalScrollbar* hbar;
        BaseVerticalScrollbar* vbar;
        int _bar_width;
        point_2d _margin;

        Grid _underly;

        void _init();

        public:
        GridWithScrollbar(const point_2d& size, int bar_width, const point_2d& margin = {64, 64});
        GridWithScrollbar(const point_2d& size, int bar_width, const attrs::ListenerMap& listeners, const point_2d& margin = {64, 64});
        GridWithScrollbar(const point_2d& size, int bar_width, attrs::ListenerMap&& listeners, const point_2d& margin = {64, 64});
        ~GridWithScrollbar();

        template <events::EventType eventtype>
        bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
        {
            if (!attrs::sizable<impl::_Grid<GridWithScrollbar>>::dispatch_templated_event<eventtype>(caller, event))
                return _underly.template dispatch_templated_event<eventtype>(caller, event);
            return true;
        }

        template <>
        bool dispatch_templated_event<events::EventType::Dragged>(const Caller&, const SDL_Event&);

        Grid& grid();
        const Grid& grid() const;

        /*
        Query if parent need to update this element on the next cycle
        */
        virtual bool need_update() const override;

        /*
        Re-render this element
        */
        virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) override;
    };
}

#endif