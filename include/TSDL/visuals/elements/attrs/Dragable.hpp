//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ATTRS_DRAGABLE_
#define TSDL_ELEMENTS_ATTRS_DRAGABLE_

#include "TSDL/visuals/elements/attrs/EventLookup.hpp"
#include "TSDL/visuals/elements/Element.hpp"
#include "TSDL/visuals/elements/ElementHolder.hpp"

#include "TSDL/Meta.hpp"

namespace TSDL::elements::attrs
{
    // function transforming starting pos and distance dragged to new pos
    using DragablePosTransformer = std::function<::TSDL::point_2d(const ::TSDL::point_2d&, const ::TSDL::point_2d&)>;

    class Dragable
    {
        private:
        ::TSDL::point_2d _pos;
        DragablePosTransformer _pos_transform_fun;

        public:
        Dragable(DragablePosTransformer pos_transform, ::TSDL::point_2d origin = {0, 0});

        ::TSDL::point_2d pos() const;
        void pos(const ::TSDL::point_2d& prev_pos, const ::TSDL::point_2d& dist);
    };

    template <typename T, typename Enable = void>
    class dragable;

    template <typename T>
    class dragable<
        T, 
        typename std::enable_if_t<
            std::is_base_of_v<DependentElement, T> &&
            std::is_base_of_v<typename dynamiceventlookup<typename T::Base_T>, T>
        >
    >:
    public T, public Dragable
    {
        private:
        bool _focused = false;
        ::TSDL::point_2d _start_pos, _ev_start_pos, _last_pos;

        public:
        template <typename ...Args>
        dragable(
            DragablePosTransformer pos_transform, 
            Args... args
        ): T(args...), Dragable(pos_transform) 
        {
            T::add_event_handler(
                ::TSDL::events::EventType::LeftDown,
                [this](const Caller&, const SDL_Event& event) -> bool
                {
                    _start_pos = pos();
                    _last_pos = _start_pos;
                    _ev_start_pos = {event.button.x, event.button.y};
                    _focused = true;
                    return true;
                }
            );

            T::add_event_handler(
                ::TSDL::events::EventType::LeftUp_Inside,
                [this](const Caller&, const SDL_Event&) -> bool
                {
                    _focused = false;
                    return true;
                }
            );

            T::add_event_handler(
                ::TSDL::events::EventType::LeftUp_Outside,
                [this](const Caller&, const SDL_Event&) -> bool
                {
                    _focused = false;
                    return true;
                }
            );

            T::add_event_handler(
                ::TSDL::events::EventType::MouseMotion,
                [this](const Caller&, const SDL_Event& event) -> bool
                {
                    if (_focused)
                    {
                        pos(_start_pos, ::TSDL::point_2d(event.motion.x, event.motion.y) - _ev_start_pos);
                        auto _this_pos = pos();
                        for (auto& hold : holder())
                        {
                            auto& _hold = hold.get();
                            _hold.move_child(*this, _this_pos - _last_pos + _hold.child_info(*this).dimension.first);
                        }
                        _last_pos = _this_pos;
                        dispatch_event(Caller(*this, {0, 0}), events::EventType::Dragged, event);
                    }
                    return true;
                }
            );
        }

        template <typename ...Args>
        dragable(
            ::TSDL::point_2d origin, 
            DragablePosTransformer pos_transform, 
            Args... args
        ): T(args...), Dragable(pos_transform, origin)
        {
            T::add_event_handler(
                ::TSDL::events::EventType::LeftDown,
                [this](const Caller&, const SDL_Event& event) -> bool
                {
                    _start_pos = pos();
                    _last_pos = _start_pos;
                    _ev_start_pos = {event.button.x, event.button.y};
                    _focused = true;
                    return true;
                }
            );

            T::add_event_handler(
                ::TSDL::events::EventType::LeftUp,
                [this](const Caller&, const SDL_Event&) -> bool
                {
                    _focused = false;
                    return true;
                }
            );

            T::add_event_handler(
                ::TSDL::events::EventType::MouseMotion,
                [this](const Caller&, const SDL_Event& event) -> bool
                {
                    if (_focused)
                    {
                        pos(_start_pos, ::TSDL::point_2d(event.motion.x, event.motion.y) - _ev_start_pos);
                        auto _this_pos = pos();
                        for (auto& hold : holder())
                        {
                            auto& _hold = hold.get();
                            _hold.move_child(*this, _this_pos - _last_pos + _hold.child_info(*this).dimension.first);
                        }
                        _last_pos = _this_pos;
                        dispatch_event(Caller(*this, {0, 0}), events::EventType::Dragged, event);
                    }
                    return true;
                }
            );
        }
    };

    template <typename T>
    class dragable<
        T,
        typename std::enable_if_t<
            std::is_base_of_v<DependentElement, T> &&
            std::is_base_of_v<typename staticeventlookup<typename T::Base_T, typename T::Derived_T>, T>
        >
    >:
    public T, public Dragable
    {
        private:
        bool _focused = false;
        ::TSDL::point_2d _start_pos, _ev_start_pos, _last_pos;

        public:
        template <typename ...Args>
        dragable(
            DragablePosTransformer pos_transform, 
            Args... args
        ): T(args...), Dragable(pos_transform) {}

        template <typename ...Args>
        dragable( 
            ::TSDL::point_2d origin, 
            DragablePosTransformer pos_transform, 
            Args... args
        ): T(args...), Dragable(pos_transform, origin) {}

        template <events::EventType eventtype>
        bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
        {
            return T::dispatch_templated_event<eventtype>(caller, event);
        }

        template <>
        bool dispatch_templated_event<events::EventType::LeftDown>(const Caller&, const SDL_Event& event)
        {
            _start_pos = pos();
            _last_pos = _start_pos;
            _ev_start_pos = {event.button.x, event.button.y};
            _focused = true;
            return true;
        }

        template <>
        bool dispatch_templated_event<events::EventType::LeftUp_Inside>(const Caller&, const SDL_Event&)
        {
            _focused = false;
            return true;
        }

        template <>
        bool dispatch_templated_event<events::EventType::LeftUp_Outside>(const Caller&, const SDL_Event&)
        {
            _focused = false;
            return true;
        }

        template <>
        bool dispatch_templated_event<events::EventType::MouseMotion>(const Caller&, const SDL_Event& event)
        {
            if (_focused)
            {
                pos(_start_pos, ::TSDL::point_2d(event.motion.x, event.motion.y) - _ev_start_pos);
                auto _this_pos = pos();
                for (auto& hold : holder())
                {
                    auto& _hold = hold.get();
                    _hold.move_child(*this, _this_pos - _last_pos + _hold.child_info(*this).dimension.first);
                }
                _last_pos = _this_pos;
                dispatch_event(Caller{*this, {0, 0}}, events::EventType::Dragged, event);
            }
            return true;
        }
    };
}

#endif