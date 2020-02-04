#include "abstract/elements/EventDispatcher.hpp"

TSDL::elements::EventDispatcher::EventDispatcher() = default;

TSDL::elements::EventDispatcher::EventDispatcher(const ListenerMap& listeners): _event_listeners(listeners) {}

TSDL::elements::EventDispatcher::EventDispatcher(ListenerMap&& listeners): _event_listeners(listeners) {}

void TSDL::elements::EventDispatcher::dispatch_event_direct(const TSDL::events::EventType& eventtype, Element& subelement)
{
    _event_listeners[eventtype].insert(std::reference_wrapper(subelement));
}

void TSDL::elements::EventDispatcher::stop_dispatch_event_direct(const TSDL::events::EventType& eventtype, Element& subelement)
{
    _event_listeners[eventtype].erase(std::reference_wrapper(subelement));
}

bool TSDL::elements::EventDispatcher::dispatch_event(const Caller& caller, const TSDL::events::EventType& eventtype, const SDL_Event& event)
{
    const Caller _this_caller = Caller(*this, caller.second);
    for(Element& subelement: _event_listeners[eventtype])
    {
        if(subelement.dispatch_event(_this_caller, eventtype, event)) return true;
    }
    return Element::dispatch_event(_this_caller, eventtype, event);
}