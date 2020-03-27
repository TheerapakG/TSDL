#include "TSDL/abstract/elements/attrs/EventForwarder.hpp"

namespace TSDL::elements::attrs
{
    EventForwarder::EventForwarder(EventLookupable& target): _target(target), EventDispatcher(ListenerMap()){}

    EventLookupable& EventForwarder::event_target() const
    {
        return _target;
    }
    
    void EventForwarder::event_target(EventLookupable& target)
    {
        _target = target;
    }
}