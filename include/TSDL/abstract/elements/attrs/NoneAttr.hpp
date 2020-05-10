//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ATTRS_NONEATTR_
#define TSDL_ELEMENTS_ATTRS_NONEATTR_

#include "TSDL/abstract/elements/Element.hpp"

namespace TSDL::elements::attrs
{
    template <class T>
    class noneattr: public T
    {
        public:
        template <typename ...Args>
        noneattr(Args... args): T(args...) {}
    };
}
#endif