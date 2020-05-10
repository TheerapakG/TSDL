//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/elements/attrs/EventForwarder.hpp"

namespace TSDL::elements::attrs
{
    EventForwarder::EventForwarder(EventLookupable& target): _target(target), EventDispatcher(ListenerMap()){}

    EventLookupable& EventForwarder::event_target() const
    {
        return _target;
    }
    
    void EventForwarder::event_target(EventLookupable& target)
    {
        _target = target;
    }
}