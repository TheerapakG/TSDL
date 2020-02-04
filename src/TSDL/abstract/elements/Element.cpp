#include "abstract/elements/Element.hpp"
#include <memory>

bool TSDL::elements::Element::operator==(const Element& other)
{
    return this == std::addressof(other);
}

void TSDL::elements::Element::update()
{
    _update = true;
}

bool TSDL::elements::Element::need_update()
{
    return _update;
}

void TSDL::elements::Element::add_event_handler(const TSDL::events::EventType& eventtype, const EventHandler& evhandler)
{
    _evhdlrmap[eventtype] = evhandler;
}

void TSDL::elements::Element::remove_event_handler(const TSDL::events::EventType& eventtype)
{
    _evhdlrmap.erase(eventtype);
}

bool TSDL::elements::Element::dispatch_event(const Caller& caller, const TSDL::events::EventType& eventtype, const SDL_Event& event)
{
    _evhdlrmap[eventtype](caller, event);
}