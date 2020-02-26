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
        using ListenerMap = std::map <const ::TSDL::events::EventType, std::set<::TSDL::elements::DependentElement*>>;
        class EventDispatcher
        {
            private:
            ListenerMap _event_listeners;

            public:
            EventDispatcher(const ListenerMap& listeners);
            EventDispatcher(ListenerMap&& listeners);

            void dispatch_event_direct(const ::TSDL::events::EventType& eventtype, DependentElement& subelement);
            void stop_dispatch_event_direct(const ::TSDL::events::EventType& eventtype, DependentElement& subelement);
            virtual bool dispatch_event(const Caller& caller, const ::TSDL::events::EventType& eventtype, const SDL_Event& event);
        };

        template <class T>
        class eventdispatcher: public T, public EventDispatcher
        {
            public:
            template <typename ...Args>
            eventdispatcher(TSDL_Renderer& renderer, Args... args): T(renderer, args...), EventDispatcher(ListenerMap()) {}
			template <typename ...Args>
            eventdispatcher(TSDL_Renderer& renderer, const ListenerMap& listeners, Args... args):
                T(renderer, args...), EventDispatcher(listeners) {}
			template <typename ...Args>
            eventdispatcher(TSDL_Renderer& renderer, ListenerMap&& listeners, Args... args):
                T(renderer, args...), EventDispatcher(listeners) {}
            template <typename U = std::enable_if_t<std::is_same_v<Element, T>>, typename ...Args>
            eventdispatcher(EventloopAdapter& evloop, Args... args): T(evloop, args...), EventDispatcher(ListenerMap()) {}
            template <typename U = std::enable_if_t<std::is_same_v<Element, T>>, typename ...Args>
            eventdispatcher(EventloopAdapter& evloop, const ListenerMap& listeners, Args... args):
                T(evloop, args...), EventDispatcher(listeners) {}
            template <typename U = std::enable_if_t<std::is_same_v<Element, T>>, typename ...Args>
            eventdispatcher(EventloopAdapter& evloop, ListenerMap&& listeners, Args... args):
                T(evloop, args...), EventDispatcher(listeners) {}
            template <typename U = std::enable_if_t<!std::is_same_v<Element, T>, EventloopAdapter>, typename ...Args>
            eventdispatcher(U& evloop, Args... args) : Element(evloop.renderer()), T(evloop, args...), EventDispatcher(ListenerMap()) {}
            template <typename U = std::enable_if_t<!std::is_same_v<Element, T>, EventloopAdapter>, typename ...Args>
            eventdispatcher(U& evloop, const ListenerMap& listeners, Args... args) :
                Element(evloop.renderer()), T(evloop, args...), EventDispatcher(listeners) {}
            template <typename U = std::enable_if_t<!std::is_same_v<Element, T>, EventloopAdapter>, typename ...Args>
            eventdispatcher(U& evloop, ListenerMap&& listeners, Args... args) :
                Element(evloop.renderer()), T(evloop, args...), EventDispatcher(listeners) {}

            virtual bool dispatch_event(
                const Caller& caller, const ::TSDL::events::EventType& eventtype, const SDL_Event& event) override
            {
                if(!EventDispatcher::dispatch_event(caller, eventtype, event)) return T::dispatch_event(caller, eventtype, event);
				return false;
            }
        };
    }
}

#endif