//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ATTREDTYPE_
#define TSDL_ELEMENTS_ATTREDTYPE_

#include "TSDL/Meta.hpp"
#include "TSDL/utilities/meta_list.hpp"

namespace TSDL::elements
{
    template <template <typename> typename Base, template <template <typename> typename...> typename Final, template <typename> typename... Attrs>
    class ATTRedType: public util::reduce<util::meta_list_template<Attrs...>, Base<Final<Attrs...>>>::type
    {
        private:
        using Attr_List = typename util::meta_list_template<Attrs...>;
        using Impl = typename Base<Final<Attrs...>>;
        using Parent = typename util::reduce<Attr_List, Impl>::type;

        public:
        friend class Final<Attrs...>;

        template <template <typename> typename Attr>
        using push_front_attr = typename Final<Attr, Attrs...>;

        template <template <typename> typename Attr>
        using push_back_attr = typename Final<Attrs..., Attr>;

        template <template <typename> typename Attr>
        using remove_attr = typename make_same_parameterized_template<Final, typename Attr_List::template remove<Attr>>::type;

        template <template <template <typename> typename> typename Predicate>
        using remove_attr_if = typename make_same_parameterized_template<Final, typename Attr_List::template remove_if<Predicate>>::type;
        
        template <typename ...Args>
        ATTRedType(Args... args): Parent(args...) {}
    };
}
#endif