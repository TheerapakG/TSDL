#include "TSDL/abstract/elements/Element.hpp"
#include <memory>

TSDL::elements::Element::Element(TSDL_Renderer& renderer): _renderer(renderer) {}

std::vector<std::reference_wrapper<::TSDL::elements::ElementHolder>> TSDL::elements::Element::holder() const
{
    return _holders;
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
    _update = false;
}

bool TSDL::elements::Element::need_update() const
{
    return _update;
}

void TSDL::elements::Element::add_event_handler(const TSDL::events::EventType& eventtype, const EventHandler& evhandler)
{
    _evhdlrmap[eventtype].emplace_back(evhandler);
}

void TSDL::elements::Element::remove_event_handler(const TSDL::events::EventType& eventtype)
{
    _evhdlrmap.erase(eventtype);
}

bool TSDL::elements::Element::dispatch_event(const Caller& caller, const TSDL::events::EventType& eventtype, const SDL_Event& event)
{
    std::vector<EventHandler> h;

    try
    {
        h = _evhdlrmap.at(eventtype);
    }
    catch(const std::out_of_range&)
    {
        return false;
    }

    bool _ret = false;

    for(auto& hi: h) _ret |= hi(caller, event);

    return _ret;
}

bool TSDL::elements::operator==(const TSDL::elements::Element& lhs, const TSDL::elements::Element& rhs)
{
    return std::addressof(lhs) == std::addressof(rhs);
}

bool TSDL::elements::operator!=(const TSDL::elements::Element& lhs, const TSDL::elements::Element& rhs)
{
    return std::addressof(lhs) != std::addressof(rhs);
}