#include "TSDL/abstract/elements/Element.hpp"
#include <memory>

TSDL::elements::Element::Element(TSDL_Renderer& renderer): _renderer(renderer) {}

bool TSDL::elements::Element::operator==(const Element& other)
{
    return this == std::addressof(other);
}

TSDL::TSDL_Renderer& TSDL::elements::Element::renderer() const
{
    return _renderer;
}

void TSDL::elements::Element::update()
{
    _update = true;
}

void TSDL::elements::Element::not_update()
{
    _update = true;
}

bool TSDL::elements::Element::need_update() const
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
    EventHandler h;

    try
    {
        h = _evhdlrmap.at(eventtype);
    }
    catch(const std::out_of_range&)
    {
        return false;
    }

    return h(caller, event);
}