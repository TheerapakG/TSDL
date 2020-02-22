#ifndef TSDL_ELEMENTS_ATTRS_DRAGABLE_
#define TSDL_ELEMENTS_ATTRS_DRAGABLE_

#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/ElementHolder.hpp"

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        namespace attrs
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

                ::TSDL::point_2d pos();
                void pos(const ::TSDL::point_2d& prev_pos, const ::TSDL::point_2d& dist);
            };

            template <class T>
            class dragable: public std::enable_if_t<std::is_base_of_v<Element, T>, T>, public Dragable
            {
                private:
                bool _focused = false;
                ::TSDL::point_2d _start_pos, _ev_start_pos, _last_pos;

                public:
                template <typename ...Args>
                dragable(
                    TSDL_Renderer& renderer, 
                    DragablePosTransformer pos_transform, 
                    Args... args
                ): Element(renderer), T(renderer, args...), Dragable(pos_transform) 
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
                            }
                            return true;
                        }
                    );
                }

                template <typename ...Args>
                dragable(
                    TSDL_Renderer& renderer, 
                    ::TSDL::point_2d origin, 
                    DragablePosTransformer pos_transform, 
                    Args... args
                ): Element(renderer), T(renderer, args...), Dragable(pos_transform, origin) 
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
                            }
                            return true;
                        }
                    );
                }
            };
        }
    }
}

#endif