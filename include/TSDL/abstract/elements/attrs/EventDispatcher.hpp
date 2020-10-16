//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ATTRS_EVENTDISPATCHER_
#define TSDL_ELEMENTS_ATTRS_EVENTDISPATCHER_

#include <set>
#include <map>
#include <functional>
#include "TSDL/Utility.hpp"
#include "TSDL/abstract/elements/attrs/EventLookup.hpp"
#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/events/EventType.hpp"

namespace TSDL::elements::attrs
{
    using ListenerMap = std::map <const ::TSDL::events::EventType, std::set<EventLookupable*>>;
    class EventDispatcher: virtual public EventLookupable
    {
        private:
        ListenerMap _event_listeners;

        public:
        EventDispatcher(const ListenerMap& listeners);
        EventDispatcher(ListenerMap&& listeners);

        void dispatch_event_direct(::TSDL::events::EventType eventtype, EventLookupable& subelement);
        void stop_dispatch_event_direct(::TSDL::events::EventType eventtype, EventLookupable& subelement);
        virtual bool dispatch_event(const Caller& caller, events::EventType eventtype, const SDL_Event& event);
    };

    template <class T>
    class eventdispatcher: public T, public EventDispatcher
    {
        public:
        template <typename ...Args>
        eventdispatcher(Args... args): T(args...), EventDispatcher(ListenerMap()) {}
        template <typename ...Args>
        eventdispatcher(const ListenerMap& listeners, Args... args):
            T(args...), EventDispatcher(listeners) {}
        template <typename ...Args>
        eventdispatcher(ListenerMap&& listeners, Args... args):
            T(args...), EventDispatcher(listeners) {}

        virtual bool dispatch_event(
            const Caller& caller, events::EventType eventtype, const SDL_Event& event) override
        {
            if (!EventDispatcher::dispatch_event(caller, eventtype, event))
            {
                if constexpr (std::is_base_of_v<EventLookupable, T>) return T::dispatch_event(caller, eventtype, event);
            }
            return false;
        }
    };
}

#endif