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
            using type = typename std::tuple_element<I, std::tuple<Types...>>::type;
        };

        using front = typename element<0>::type;        

        using back = typename element<size-1>::type;

        static constexpr bool empty = (size==0);

        template <typename T>
        using push_front = typename meta_list<T, Types...>;

        template <typename T>
        using push_back = typename meta_list<Types..., T>;

        // merge two meta_list together
        template <typename T, std::enable_if_t<traits::is_parameterized_template_v<T, meta_list>>>
        using merge = typename make_merge_parameterized<meta_list, std::tuple<Types...>, T>::type;

        /*
        Predicate<type>::value must yield constexpr bool value
        */
        template <template <typename> typename Predicate>
        using remove_if = typename remove_parameterized<Predicate, meta_list<Types...>>;

        template <typename T>
        using remove = typename remove_if<partial<std::is_same, T>::type>;
    };

    template <template <typename...> typename... Templates>
    struct meta_list_template
    {
        static constexpr size_t size = sizeof...(Templates);

        template <size_t I>
        struct element
        {
            static_assert(I < size, "error: invalid element index.");

            template <size_t I, template <typename...> typename _First_Template, template <typename...> typename _Rest_Template>
            struct _element_iter
            {
                using next = _element_iter<I - 1, _Rest_Template...>;

                template <typename... Types>
                using type = typename next::template type<Types...>;
            };

            template <template <typename...> typename _First_Template, template <typename...> typename _Rest_Template>
            struct _element_iter<0, _First_Template, _Rest_Template...>
            {
                template <typename... Types>
                using type = typename _First_Template<Types...>;
            };

            using _element_ptr = typename _element_iter<I, Templates...>;

            template <typename... Types>
            using type = typename _element_ptr::template type<Types...>;
        };

        template <typename... Types>
        using front = typename element<0>::template type<Types...>;        

        template <typename... Types>
        using back = typename element<size-1>::template type<Types...>;

        static constexpr bool empty = (size==0);

        template <template <typename...> typename T>
        using push_front = typename meta_list_template<T, Templates...>;

        template <template <typename...> typename T>
        using push_back = typename meta_list_template<Templates..., T>;
        
        // merge two meta_list together
        template <typename T, std::enable_if_t<traits::is_parameterized_template<T, meta_list>>>
        using merge = typename make_merge_parameterized_template<meta_list_template, meta_list_template<Types...>, T>::type;

        /*
        Predicate<type>::value must yield constexpr bool value
        */
        template <template <template <typename...> typename> typename Predicate>
        using remove_if = typename remove_parameterized_template<Predicate, meta_list_template<Types...>>;

        template <template <typename...> typename T>
        using remove = typename remove_if<partial_template<is_same_template, T>::type>;
    };

    template <typename Meta_Template_Container, typename T>
    struct reduce
    {
        template <typename _Meta_Template_Container>
        struct _reduced_type {};

        template <
            template <template <typename> typename...> typename Meta_Template_Container_Template,
            template <typename> typename... Templates
        >
        struct _reduced_type<Meta_Template_Container_Template<Templates...>>
        {
            template <template <typename> typename... _Templates>
            struct _template_reducer
            {
                using type = T;
            };

            template <template <typename> typename _First_Template, template <typename> typename... _Rest_Template>
            struct _template_reducer<_First_Template, _Rest_Template...>
            {
                using type = typename _First_Template<T>;
            };

            template <template <typename> typename _First_Template, template <typename> typename _Second_Template, template <typename> typename... _Rest_Template>
            struct _template_reducer<_First_Template, _Second_Template, _Rest_Template...>
            {
                template <typename _T>
                using _reduced_pair = typename _First_Template<typename _Second_Template<_T>>;

                using type = typename _template_reducer<_reduced_pair, _Rest_Template...>::type;
            };

            using type = typename _template_reducer<Templates...>::type;
        };

        using type = typename _reduced_type<Meta_Template_Container>::type;
    };
}

#endif