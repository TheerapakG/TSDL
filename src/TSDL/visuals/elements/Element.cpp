//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/visuals/elements/Element.hpp"
#include "TSDL/visuals/elements/EventloopAdapter.hpp"
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