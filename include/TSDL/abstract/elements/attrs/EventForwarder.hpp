//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ATTRS_EVENTFORWARDER_
#define TSDL_ELEMENTS_ATTRS_EVENTFORWARDER_

#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/abstract/elements/attrs/EventLookup.hpp"
#include <functional>

namespace TSDL::elements::attrs
{
    class EventForwarder: public EventDispatcher
    {
        private:
        std::reference_wrapper<EventLookupable> _target;

        public:
        EventForwarder(EventLookupable& target);

        EventLookupable& event_target() const;
        void event_target(EventLookupable& target);
    };

    template <class T>
    class eventforwarder: 
    public std::enable_if_t<
        std::is_base_of_v<DependentElement, T>, 
        T
    >, 
    public EventForwarder
    {
        public:
        template <typename ...Args>
        eventforwarder(EventLookupable& target, Args... args): T(args...), EventForwarder(target) {}

        virtual bool dispatch_event(const Caller& caller, events::EventType eventtype, const SDL_Event& event) override
        {
            return event_target().dispatch_event(Caller{std::reference_wrapper<EventDispatcher>(*this), caller.event_location}, eventtype, event);
        }
    };
}
#endif