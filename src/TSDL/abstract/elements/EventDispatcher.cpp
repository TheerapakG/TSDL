#include "TSDL/abstract/elements/EventDispatcher.hpp"

TSDL::elements::EventDispatcher::EventDispatcher(const ListenerMap& listeners): 
    _event_listeners(listeners) {}

TSDL::elements::EventDispatcher::EventDispatcher(ListenerMap&& listeners): 
    _event_listeners(listeners) {}

void TSDL::elements::EventDispatcher::dispatch_event_direct(const TSDL::events::EventType& eventtype, DependentElement& subelement)
{
    _event_listeners[eventtype].insert(&subelement);
}

void TSDL::elements::EventDispatcher::stop_dispatch_event_direct(const TSDL::events::EventType& eventtype, DependentElement& subelement)
{
    _event_listeners[eventtype].erase(&subelement);
}

bool TSDL::elements::EventDispatcher::dispatch_event(const Caller& caller, const TSDL::events::EventType& eventtype, const SDL_Event& event)
{
    const Caller _this_caller = Caller(*this, caller.second);
    for(DependentElement* subelement: _event_listeners[eventtype])
    {
        if(subelement->dispatch_event(_this_caller, eventtype, event)) return true;
    }
    return false;
}