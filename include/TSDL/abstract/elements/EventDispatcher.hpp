#ifndef TSDL_ELEMENTS_EVENTDISPATCHER_
#define TSDL_ELEMENTS_EVENTDISPATCHER_

#include <set>
#include <map>
#include <functional>
#include "TSDL/TSDL_Utility.hpp"
#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/events/EventType.hpp"

namespace TSDL
{
    namespace elements
    {
        using ListenerMap = std::map <const ::TSDL::events::EventType, std::set<Element*>>;
        class EventDispatcher: virtual public Element
        {
            private:
            ListenerMap _event_listeners;

            public:
            EventDispatcher(TSDL_Renderer& renderer);
            EventDispatcher(TSDL_Renderer& renderer, const ListenerMap& listeners);
            EventDispatcher(TSDL_Renderer& renderer, ListenerMap&& listeners);

            void dispatch_event_direct(const ::TSDL::events::EventType& eventtype, Element& subelement);
            void stop_dispatch_event_direct(const ::TSDL::events::EventType& eventtype, Element& subelement);
            virtual bool dispatch_event(const Caller& caller, const ::TSDL::events::EventType& eventtype, const SDL_Event& event) override;
        };
    }
}

#endif