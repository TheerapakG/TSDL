//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_SCENE_
#define TSDL_ELEMENTS_SCENE_

#include "TSDL/visuals/elements/Element.hpp"
#include "TSDL/visuals/elements/WindowAdapter.hpp"
#include "TSDL/visuals/elements/attrs/EventLookup.hpp"

namespace TSDL::elements
{
    /*
    * SceneCommon provides common base for Scenes
    */
   class SceneCommon
   {
       public:
       virtual ~SceneCommon() = default;
   };

    /*
    * Scene is a class that helps binding and unbinding target to the WindowAdapter
    */
    template <typename T, typename Derived>
    class Scene: public SceneCommon, public attrs::staticeventlookup<DependentElement, Derived>
    {
        private:
        optional_reference<WindowAdapter> _window;

        public:
        using target_t = typename T;

        Scene(): attrs::staticeventlookup<DependentElement, Derived>{} {};
        Scene(const Scene&) = delete;
        Scene(Scene&&) = default;

        virtual T& target() = 0;
        virtual const T& target() const = 0;
        virtual void on_unbound_window(WindowAdapter& window) = 0;

        /*
        * Set window's render source to the scene.
        * Functionally equivalent to window.src(scene). Provided for readability.
        */
        void bind_to(WindowAdapter& window)
        {
            if (_window)
            {
                // TODO: throw
                return;
            }
            window.src(*this);
            _window = window;
        }

        template<events::EventType eventtype>
        bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
        {
            return target().dispatch_event(caller, eventtype, event);
        }

        template<>
        bool dispatch_templated_event<events::EventType::UnboundWindow>(const Caller& caller, const SDL_Event& event)
        {
            _window.reset();
            return target().dispatch_event(caller, events::EventType::UnboundWindow, event);
        }

        WindowAdapter& bounded_window()
        {
            return get_ref(_window);
        }

        virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) const final
        {
            target().render(window, dist);
        }
    };

    /*
    * InstantBindScene is a Scene that automatically bind to window on instantiation
    */
    template <typename T, typename Derived>
    class InstantBindScene : public Scene<T, Derived>
    {
        public:
        InstantBindScene(WindowAdapter& window): Scene<T, Derived>()
        {
            bind_to(window);
        }
    };
}

#endif