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