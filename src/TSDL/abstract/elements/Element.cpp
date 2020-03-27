#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include <memory>

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