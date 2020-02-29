#ifndef TSDL_ELEMENTS_ATTRS_GRIDDED_
#define TSDL_ELEMENTS_ATTRS_GRIDDED_

#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/Grid.hpp"

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL::elements::attrs
{
    class Gridded
    {
        private:
        Grid _grid;

        public:
        Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer);
        Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ListenerMap& listeners);
        Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer, ListenerMap&& listeners);

        Grid& grid();
        const Grid& grid() const;
    };

    template <class T>
    class gridded: public std::enable_if_t<std::is_base_of_v<DependentElement, T>, T>, public Gridded
    {
        public:
        template <typename ...Args>
        gridded(
            EventloopAdapter& evloop, 
            TSDL_Renderer& renderer, 
            Args... args
        ): T(evloop, renderer, args...), Gridded(evloop, renderer) 
        {
            Element::add_event_handler(
                ::TSDL::events::EventType::MouseMotion,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MouseMotion, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftUp_Outside, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightUp_Outside, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleUp_Outside, event);
                }
            );
        }

        template <typename ...Args>
        gridded(
            EventloopAdapter& evloop,
            TSDL_Renderer& renderer, 
            const ListenerMap& listeners,
            Args... args
        ): T(evloop, renderer, args...), Gridded(evloop, renderer, listeners)
        {
            Element::add_event_handler(
                ::TSDL::events::EventType::MouseMotion,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MouseMotion, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftUp_Outside, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightUp_Outside, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleUp_Outside, event);
                }
            );
        }

        template <typename ...Args>
        gridded(
            EventloopAdapter& evloop,
            TSDL_Renderer& renderer, 
            ListenerMap&& listeners,
            Args... args
        ): T(evloop, renderer, args...), Gridded(evloop, renderer, listeners)
        {
            Element::add_event_handler(
                ::TSDL::events::EventType::MouseMotion,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MouseMotion, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::LeftUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::LeftUp_Outside, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::RightUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::RightUp_Outside, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleDown,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleDown, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleUp,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleUp, event);
                }
            );

            Element::add_event_handler(
                ::TSDL::events::EventType::MiddleUp_Outside,
                [this](const Caller& caller, const SDL_Event& event) -> bool
                { 
                    Grid& _grid = grid();
                    return _grid.dispatch_event(Caller(_grid, caller.second), ::TSDL::events::EventType::MiddleUp_Outside, event);
                }
            );
        }
    };
}

#endif