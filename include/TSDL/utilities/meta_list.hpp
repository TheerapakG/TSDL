//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_UTILITIES_META_LIST
#define TSDL_UTILITIES_META_LIST

#include <TSDL/TSDL_Meta.hpp>
#include <tuple>

namespace TSDL::util
{
    template <typename... Types>
    struct meta_list
    {
        static constexpr size_t size = sizeof...(Types);

        template <size_t I>
        struct element
        {
            static_assert(I < size, "error: invalid element index.");
            using type = std::tuple_element<I, std::tuple<Types...>>::type;
        }

        using front = typename element<0>;        

        using back = typename element<size-1>;

        static constexpr bool empty = (size==0);

        template <typename T>
        using push_front = typename meta_list<T, Types...>;

        template <typename T>
        using push_back = typename meta_list<Types..., T>;

        // merge two meta_list together
        template <typename T, std::enable_if_t<traits::is_parameterized_template<T, meta_list>>>
        using merge = typename make_merge_parameterized<meta_list, std::tuple<Types...>, T>::type;      
    };
}

#endif