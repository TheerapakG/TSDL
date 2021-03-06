//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ELEMENT_
#define TSDL_ELEMENTS_ELEMENT_

#include <atomic>
#include <vector>
#include <map>
#include <functional>
#include "TSDL/Utility.hpp"
#include "TSDL/Texture.hpp"
#include "TSDL/Renderer.hpp"
#include "TSDL/visuals/events/EventType.hpp"

namespace TSDL
{
    namespace elements
    {
        class Element;

        class EventloopAdapter;
        class WindowAdapter;
        class ElementHolder;

        namespace attrs
        {
            class EventLookupable;
        }

        struct Caller
        {
            attrs::EventLookupable& event_origin;
            point_2d event_location;
        };

        using EventHandler = std::function<bool(const Caller&, const SDL_Event&)>;

        static EventHandler always_true_event_handler = [](const Caller&, const SDL_Event&){ return true; };
        
        class Element
        {
            public:
            Element() = default;
            Element(const Element&) = delete;
        };
        
        bool operator==(const Element& lhs, const Element& rhs);
        bool operator!=(const Element& lhs, const Element& rhs);

        class DependentElement: virtual public Element
        {
            private:
            mutable std::atomic<bool> _update = true;
            std::vector<std::reference_wrapper<::TSDL::elements::ElementHolder>> _holders;

            public:
            friend class EventloopAdapter;
            friend class ElementHolder;

            /*
            Get bounded holder
            */
            std::vector<std::reference_wrapper<::TSDL::elements::ElementHolder>> holder() const;

            /*
            Make parent update this element on the next cycle
            */
            void update();

            /*
            Reverse what update() did after this cycle finished but before the next cycle begins
            */
            void not_update() const;

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const;

            /*
            Re-render this element
            */
            virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) const = 0;
        };

        class RenderSizedElement: public DependentElement
        {
            public:
            using DependentElement::DependentElement;

            /*
            Forces this element to be rendered with specified size
            */
            virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size) const = 0;
        };
    }
}

#endif