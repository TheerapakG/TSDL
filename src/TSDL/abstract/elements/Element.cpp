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

std::vector<std::reference_wrapper<::TSDL::elements::ElementHolder>> TSDL::elements::DependentElement::holder() const
{
    return _holders;
}

void TSDL::elements::DependentElement::update()
{
    _update = true;
}

void TSDL::elements::DependentElement::not_update()
{
    current_eventloop_adapter().register_not_update(*this);
}

bool TSDL::elements::DependentElement::need_update() const
{
    return _update;
}