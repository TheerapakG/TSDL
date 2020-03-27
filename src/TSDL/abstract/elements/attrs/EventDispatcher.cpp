#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include <utility>

TSDL::elements::attrs::EventDispatcher::EventDispatcher(const ListenerMap& listeners): 
    _event_listeners(listeners) {}

TSDL::elements::attrs::EventDispatcher::EventDispatcher(ListenerMap&& listeners): 
    _event_listeners(listeners) {}

void TSDL::elements::attrs::EventDispatcher::dispatch_event_direct(TSDL::events::EventType eventtype, EventLookupable& subelement)
{
    _event_listeners[eventtype].insert(&subelement);
}

void TSDL::elements::attrs::EventDispatcher::stop_dispatch_event_direct(TSDL::events::EventType eventtype, EventLookupable& subelement)
{
    _event_listeners[eventtype].erase(&subelement);
}

bool TSDL::elements::attrs::EventDispatcher::dispatch_event(const Caller& caller, events::EventType eventtype, const SDL_Event& event)
{
    const Caller _this_caller = Caller(*this, caller.second);
    for(EventLookupable* subelement: _event_listeners[eventtype])
    {
        if(subelement->dispatch_event(_this_caller, eventtype, event)) return true;
    }
    return false;
}