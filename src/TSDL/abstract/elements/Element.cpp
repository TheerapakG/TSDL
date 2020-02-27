#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include <memory>

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

void TSDL::elements::Element::add_event_handler(const TSDL::events::EventType& eventtype, const EventHandler& evhandler)
{
    _evhdlrmap[eventtype].emplace_back(evhandler);
}

void TSDL::elements::Element::remove_event_handler(const TSDL::events::EventType& eventtype)
{
    _evhdlrmap.erase(eventtype);
}

bool TSDL::elements::operator==(const TSDL::elements::Element& lhs, const TSDL::elements::Element& rhs)
{
    return std::addressof(lhs) == std::addressof(rhs);
}

bool TSDL::elements::operator!=(const TSDL::elements::Element& lhs, const TSDL::elements::Element& rhs)
{
    return std::addressof(lhs) != std::addressof(rhs);
}

TSDL::elements::DependentElement::DependentElement(EventloopAdapter& evloop, TSDL_Renderer& renderer):
    _evloop(evloop), _renderer(renderer) {}

std::vector<std::reference_wrapper<::TSDL::elements::ElementHolder>> TSDL::elements::DependentElement::holder() const
{
    return _holders;
}

TSDL::TSDL_Renderer& TSDL::elements::DependentElement::renderer() const
{
    return _renderer;
}

TSDL::elements::EventloopAdapter& TSDL::elements::DependentElement::eventloop() const
{
    return _evloop;
}

void TSDL::elements::DependentElement::update()
{
    _update = true;
}

void TSDL::elements::DependentElement::not_update()
{
    _evloop.register_not_update(*this);
}

bool TSDL::elements::DependentElement::need_update() const
{
    return _update;
}