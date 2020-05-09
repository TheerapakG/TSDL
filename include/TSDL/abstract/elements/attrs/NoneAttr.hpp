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