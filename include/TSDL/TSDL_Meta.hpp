//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_META_
#define TSDL_META_

#include <type_traits>
#include <optional>
#include <functional>
#include <tuple>
#include <iterator>
#include <utility>

#if __cplusplus > 201703L
#include <version>
#ifdef __cpp_lib_concepts
#include <concepts>
#endif
#endif

namespace TSDL
{
    namespace impl
    {
        template <template <typename...> typename... Templates>
        struct _template_holder {};
    }

    template <bool u, bool ...v>
    struct _and
    {
        static constexpr bool value = u && _and<v...>::value;
    };

    template <>
    struct _and <true> : std::true_type {};

    template <>
    struct _and <false> : std::false_type {};

    template <bool ...v>
    inline constexpr bool _and_v = _and<v...>::value;

    template <bool u, bool ...v>
    struct _or
    {
        static constexpr bool value = u || _or<v...>::value;
    };

    template <>
    struct _or <true> : std::true_type {};

    template <>
    struct _or <false> : std::false_type {};

    template <bool ...v>
    inline constexpr bool _or_v = _or<v...>::value;

    template <typename T>
    struct remove_all_pointers
    {
        using type = T;
    };

    template <typename T>
    struct remove_all_pointers<T*>
    {
        using type = typename remove_all_pointers<T>::type;
    };

    template <typename T>
    struct remove_all_pointers<T* const>
    {
        using type = typename remove_all_pointers<T>::type;
    };

    template <typename T>
    struct remove_all_pointers<T* volatile>
    {
        using type = typename remove_all_pointers<T>::type;
    };

    template <typename T>
    struct remove_all_pointers<T* const volatile>
    {
        using type = typename remove_all_pointers<T>::type;
    };

    template <typename T>
    using remove_all_pointers_t = typename remove_all_pointers<T>::type;

    template <typename T>
    struct return_type { using type = typename std::function<T>::result_type; };

    template <typename T>
    using return_type_t = typename return_type<T>::type;

    template <typename F>
    struct function_traits;

    // function pointer
    template <typename R, typename... Args>
    struct function_traits<R(*)(Args...)> : function_traits<R(Args...)> {};

    template <typename R, typename... Args>
    struct function_traits<R(Args...)>
    {
        using return_type = R;

        static constexpr std::size_t arity = sizeof...(Args);

        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "invalid parameter index.");
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };
    };

    // member function pointer
    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...)> : function_traits<R(C&,Args...)> {};

    // const member function pointer
    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...) const> : function_traits<R(C&,Args...)> {};

    // member object pointer
    template <typename C, typename R>
    struct function_traits<R(C::*)> : public function_traits<R(C&)> {};

    // functor
    template <typename F>
    struct function_traits
    {
        private:
            using call_type = function_traits<decltype(&F::operator())>;
        public:
            using return_type = typename call_type::return_type;

            static constexpr std::size_t arity = call_type::arity - 1;

            template <std::size_t N>
            struct argument
            {
                static_assert(N < arity, "error: invalid parameter index.");
                using type = typename call_type::template argument<N+1>::type;
            };
    };

    template <typename F>
    struct function_traits<F&> : function_traits<F> {};

    template <typename F>
    struct function_traits<F&&> : function_traits<F> {};

    template <typename F, std::size_t i>
    struct get_argument : function_traits<F>::template argument<i> {};

    template <typename F, std::size_t i>
    using get_argument_t = typename get_argument<F, i>::type;

    template <typename T, typename R, typename... A> struct ptr_fun
    {
        template <R(T::*FUNC)(A...)> struct make
        {
            static R fun(T* p, A... args) { return (p->*FUNC)(args...); }
        };

        template <R(T::*FUNC)(A...) const> struct make_constant
        {
            static R fun(const T* p, A... args) { return (p->*FUNC)(args...); }
        };

        template <R(T::*FUNC)(A...)> struct make_void
        {
            static R fun(void* p, A... args) { return (static_cast<T*>(p)->*FUNC)(args...); }
        };

        template <R(T::*FUNC)(A...)> struct make_constant_void
        {
            static R fun(const void* p, A... args) { return (static_cast<const T*>(p)->*FUNC)(args...); }
        };
    };

    template <typename T>
    struct reversion_wrapper { T& iterable; };

    template <typename T>
    auto begin (reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

    template <typename T>
    auto end (reversion_wrapper<T> w) { return std::rend(w.iterable); }

    template <typename T>
    reversion_wrapper<T> reverse (T&& iterable) { return { iterable }; }

    template <typename T>
    using optional_reference = typename std::optional<std::reference_wrapper<typename std::remove_reference_t<T>>>;

    template <typename T>
    optional_reference<typename std::remove_reference_t<T>> make_optional_ref(T&& ref)
    {
        return ref;
    }

    template <typename T>
    optional_reference<typename std::remove_reference_t<T>> make_optional_ref()
    {
        return optional_reference<T>();
    }

    template <
        typename T, 
        typename _Return_T = T::value_type::type, 
        typename _SNIFAE = std::enable_if_t<std::is_same_v<T, optional_reference<_Return_T>>>
    >
    _Return_T& get_ref(const T& stdref)
    {
        return stdref.value().get();
    }

    template <typename T>
    using optional_const_reference = typename optional_reference<typename std::add_const_t<typename std::remove_reference_t<T>>>;

    template <typename T>
    optional_const_reference<typename std::remove_reference_t<T>> make_optional_const_ref(T&& ref)
    {
        return ref;
    }

    template <typename T>
    optional_const_reference<typename std::remove_reference_t<T>> make_optional_const_ref()
    {
        return optional_const_reference<T>();
    }

    template <typename T>
    struct unreferenced_type {};

    template <typename T>
    struct unreferenced_type<std::optional<std::reference_wrapper<T>>>
    {
        using type = T;
    };

    template <typename T>
    using unreferenced_type_t = typename unreferenced_type<T>::type;

    template <typename T>
    using transformer = std::function<T(const T&)>;

    template <template <typename...> typename T, template <typename...> typename U>
    struct is_same_template: std::false_type {};

    template <template <typename...> typename T>
    struct is_same_template<T, T>: std::true_type {};

    template <template <template <typename...> typename, template <typename...> typename> typename T>
    struct as_variadic_template_template
    {
        template <template <typename...> typename... Types>
        using type = typename T<Types...>;
    };

    template <template <typename...> typename Template, typename... Types>
    struct partial
    {
        template <typename... Rest_Types>
        using type = typename Template<Types..., Rest_Types...>;
    };

    template <typename T>
    using is_type = partial<std::is_same, T>;

    template <template <template <typename...> typename...> typename Template, template <typename...> typename... Types>
    struct partial_template
    {
        template <template <typename...> typename... Rest_Types>
        using type = typename Template<Types..., Rest_Types...>;
    };

    template <template <typename...> typename T>
    using is_template = partial_template<as_variadic_template_template<is_same_template>::type, T>;

    /*
    Make T parameterized by the same template argument of U
    */
    template <template <typename...> typename T, typename U>
    struct make_same_parameterized {}; // U is not a parametized template

    /*
    Make T parameterized by the same template argument of U
    */
    template <template <typename...> typename T, template <typename...> typename _U, typename... _UTypes>
    struct make_same_parameterized<T, _U<_UTypes...>>
    {
        using type = T<_UTypes...>;
    };

    /*
    Make T parameterized by the same template argument of U
    */
    template <template <template <typename...> typename...> typename T, typename U>
    struct make_same_parameterized_template {}; // U is not a parametized template

    /*
    Make T parameterized by the same template argument of U
    */
    template <template <template <typename...> typename...> typename T, template <template <typename...> typename...> typename _U, template <typename...> typename... _UTypes>
    struct make_same_parameterized_template<T, _U<_UTypes...>>
    {
        using type = T<_UTypes...>;
    };

    /*
    Make T parameterized by the same template argument of Us joined together
    */
    template <template <typename...> typename T, typename... Us>
    struct make_merge_parameterized {}; // One of the U is not a parametized template

    /*
    Make T parameterized by the same template argument of Us joined together
    */
    template <template <typename...> typename T, typename U>
    struct make_merge_parameterized<T, U>
    {
        using type = typename make_same_parameterized<T, U>::type;
    };

    /*
    Make T parameterized by the same template argument of Us joined together
    */
    template <
        template <typename...> typename T, 
        template <typename...> typename _First_U, 
        template <typename...> typename _Second_U, 
        typename... _First_UTypes, 
        typename... _Second_UTypes, 
        typename... _Rest_U
    >
    struct make_merge_parameterized<T, _First_U<_First_UTypes...>, _Second_U<_Second_UTypes...>, _Rest_U...>
    {
        using type = typename make_merge_parameterized<T, std::tuple<_First_UTypes..., _Second_UTypes...>, _Rest_U...>::type;
    };

    /*
    Make T parameterized by the same template argument of Us joined together
    */
    template <template <template <typename...> typename...> typename T, typename... Us>
    struct make_merge_parameterized_template {}; // One of the U is not a parametized template

    /*
    Make T parameterized by the same template argument of Us joined together
    */
    template <template <template <typename...> typename...> typename T, typename U>
    struct make_merge_parameterized_template<T, U>
    {
        using type = typename make_same_parameterized_template<T, U>::type;
    };

    /*
    Make T parameterized by the same template argument of Us joined together
    */
    template <
        template <template <typename...> typename...> typename T, 
        template <template <typename...> typename...> typename _First_U, 
        template <template <typename...> typename...> typename _Second_U, 
        template <typename...> typename... _First_UTypes, 
        template <typename...> typename... _Second_UTypes, 
        typename... _Rest_U
    >
    struct make_merge_parameterized_template<T, _First_U<_First_UTypes...>, _Second_U<_Second_UTypes...>, _Rest_U...>
    {
        using type = typename make_merge_parameterized_template<T, impl::_template_holder<_First_UTypes..., _Second_UTypes...>, _Rest_U...>::type;
    };

    /*
    Remove all template argument of parameterized template U satisfying Predicate
    Predicate<type>::value must yield constexpr bool value
    */
    template <template <typename> typename Predicate, typename U>
    struct remove_parameterized {}; // U is not a parametized template

    /*
    Remove all template argument of parameterized template U satisfying Predicate
    Predicate<type>::value must yield constexpr bool value
    */
    template <template <typename> typename Predicate, template <typename...> typename _U, typename... _UTypes>
    struct remove_parameterized<Predicate, _U<_UTypes...>>
    {
        template <typename _Removed_Tpl, typename... _Rest_UTypes>
        struct _remove_iterator {};

        template <typename _Removed_Tpl, typename _First_UTypes, typename... _Rest_UTypes>
        struct _remove_iterator<_Removed_Tpl, _First_UTypes, _Rest_UTypes...>
        {
            template <typename _SNIFAE = void>
            struct _remove_iterator_remove_current
            {
                template <typename _Tpl>
                struct _push_back__first_utypes_tpl {}; // Tpl is not a parametized template

                template <template <typename...> typename __Tpl, typename... __TplTypes>
                struct _push_back__first_utypes_tpl<typename __Tpl<__TplTypes...>>
                {
                    using type = typename __Tpl<__TplTypes..., _First_UTypes>;
                };

                using type = typename _remove_iterator<typename _push_back__first_utypes_tpl<_Removed_Tpl>::type, _Rest_UTypes...>::type;
            };

            template <>
            struct _remove_iterator_remove_current<std::enable_if_t<Predicate<_First_UTypes>::value>>
            {
                using type = typename _remove_iterator<_Removed_Tpl, _Rest_UTypes...>::type;
            };

            using type = typename _remove_iterator_remove_current<>::type;
        };

        template <typename _Removed_Tpl>
        struct _remove_iterator<_Removed_Tpl>
        {
            using type = typename make_same_parameterized<_U, _Removed_Tpl>::type;
        };

        using type = typename _remove_iterator<std::tuple<>, _UTypes...>::type;
    };

    /*
    Remove all template template argument of parameterized template U satisfying Predicate
    */
    template <template <template <typename...> typename> typename Predicate, typename U>
    struct remove_parameterized_template {}; // U is not a parametized template

    /*
    Remove all template template argument of parameterized template U satisfying Predicate
    Predicate<type>::value must yield constexpr bool value
    */
    template <template <template <typename...> typename> typename Predicate, template <template <typename...> typename...> typename _U, template <typename...> typename... _UTypes>
    struct remove_parameterized_template<Predicate, _U<_UTypes...>>
    {
        template <typename _Removed_Tpl, template <typename...> typename... _Rest_UTypes>
        struct _remove_iterator {};

        template <typename _Removed_Tpl, template <typename...> typename _First_UTypes, template <typename...> typename... _Rest_UTypes>
        struct _remove_iterator<_Removed_Tpl, _First_UTypes, _Rest_UTypes...>
        {
            template <typename _SNIFAE = void>
            struct _remove_iterator_remove_current
            {
                template <typename _Tpl>
                struct _push_back__first_utypes_tpl {}; // Tpl is not a parametized template template

                template <template <template <typename...> typename...> typename __Tpl, template <typename...> typename... __TplTypes>
                struct _push_back__first_utypes_tpl<typename __Tpl<__TplTypes...>>
                {
                    using type = typename __Tpl<__TplTypes..., _First_UTypes>;
                };

                using type = typename _remove_iterator<typename _push_back__first_utypes_tpl<_Removed_Tpl>::type, _Rest_UTypes...>::type;
            };

            template <>
            struct _remove_iterator_remove_current<std::enable_if_t<Predicate<_First_UTypes>::value>>
            {
                using type = typename _remove_iterator<_Removed_Tpl, _Rest_UTypes...>::type;
            };

            using type = typename _remove_iterator_remove_current<>::type;
        };

        template <typename _Removed_Tpl>
        struct _remove_iterator<_Removed_Tpl>
        {
            using type = typename make_same_parameterized_template<_U, _Removed_Tpl>::type;
        };

        using type = typename _remove_iterator<impl::_template_holder<>, _UTypes...>::type;
    };
}

namespace TSDL::traits
{
    template <typename T, typename Enable = void>
    struct is_callable_not_overloaded
    {
        static constexpr bool value = false;
    };
    
    template <typename T>
    struct is_callable_not_overloaded <T, std::enable_if_t<std::is_same_v<decltype(void(&T::operator())), void>>>
    {
        static constexpr bool value = true;
    };
    
    template <typename T>
    struct is_callable_not_overloaded <T, std::enable_if_t<std::is_function_v<remove_all_pointers_t<T>>>>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    inline constexpr bool is_callable_not_overloaded_v = is_callable_not_overloaded<T>::value;

    #if defined(__cpp_concepts) && defined(__cpp_lib_concepts)
    /*
    Concept for checking if T is an input iterator yielding type of U
    */
    template <typename T, typename U>
    concept InputIterator = requires (T t)
    {
        requires std::input_iterator<T>;
        requires std::same_as<std::iter_value_t<T>, U>;
    };
    #endif

    /*
    Check if T is an input iterator yielding type of U
    */
    template <typename T, typename U, typename Enable = void>
    struct is_input_iterator
    {
        static constexpr bool value = false;
    };

    /*
    Check if T is an input iterator yielding type of U
    */
    #if defined(__cpp_concepts) && defined(__cpp_lib_concepts)
    template <typename T, typename U> requires InputIterator<T, U>
    struct is_input_iterator <T, U, void>
    {
        static constexpr bool value = true;
    };
    #else
    template <typename T, typename U>
    struct is_input_iterator <T, U,
        std::enable_if_t<
            _and_v<
                std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>,
                std::is_same_v<typename std::iterator_traits<T>::value_type, U>
            >
        >
    >
    {
        static constexpr bool value = true;
    };
    #endif

    /*
    Helper for is_input_iterator
    */
    template <typename T, typename U>
    inline constexpr bool is_input_iterator_v = is_input_iterator<T, U>::value;



    #if defined(__cpp_concepts) && defined(__cpp_lib_concepts)
    /*
    Concept for checking if T is an input iterator yielding type convertible to U
    */
    template <typename T, typename U>
    concept InputIteratorConvertible = requires (T t)
    {
        requires std::input_iterator<T>;
        requires std::derived_from<std::iter_value_t<T>, U>;
    };
    #endif

    /*
    Check if T is an input iterator yielding type convertible to U
    */
    template <typename T, typename U, typename Enable = void>
    struct is_input_iterator_convertible
    {
        static constexpr bool value = false;
    };

    /*
    Check if T is an input iterator yielding type convertible to U
    */
    #if defined(__cpp_concepts) && defined(__cpp_lib_concepts)
    template <typename T, typename U> requires InputIteratorConvertible<T, U>
    struct is_input_iterator_convertible <T, U, void>
    {
        static constexpr bool value = true;
    };
    #else
    template <typename T, typename U>
    struct is_input_iterator_convertible <T, U,
        std::enable_if_t<
            _and_v<
                std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>,
                std::is_base_of_v<U, typename std::iterator_traits<T>::value_type>
            >
        >
    >
    {
        static constexpr bool value = true;
    };
    #endif

    /*
    Helper for is_input_iterator_convertible
    */
    template <typename T, typename U>
    inline constexpr bool is_input_iterator_convertible_v = is_input_iterator_convertible<T, U>::value;



    #if defined(__cpp_concepts) && defined(__cpp_lib_concepts)
    template <typename T>
    concept ForwardIterable = requires (T t)
    {
        typename T::iterator;
        requires std::forward_iterator<typename T::iterator>;
        requires std::same_as<decltype(std::begin(t)), typename T::iterator>;
        requires std::same_as<decltype(std::end(t)), typename T::iterator>;
    };
    #endif

    template <typename T, typename Enable = void>
    struct is_forward_iterable
    {
        static constexpr bool value = false;
    };

    #if defined(__cpp_concepts) && defined(__cpp_lib_concepts)
    template <typename T> requires ForwardIterable<T>
    struct is_forward_iterable <T, void>
    {
        static constexpr bool value = true;
    };
    #else
    template <typename T>
    struct is_forward_iterable <T, 
        std::enable_if_t<
            _and_v<
                std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<typename T::iterator>::iterator_category>,
                std::is_same_v<decltype(std::begin(std::declval<T>())), typename T::iterator>,
                std::is_same_v<decltype(std::end(std::declval<T>())), typename T::iterator>
            >
        >
    >
    {
        static constexpr bool value = true;
    };
    #endif

    template <typename T>
    inline constexpr bool is_forward_iterable_v = is_forward_iterable<T>::value;

    /*
    Check if T is parameterized type of template U
    */
    template <typename T, template <typename...> typename U>
    struct is_parameterized_template
    {
        static constexpr bool value = false;
    };

    /*
    Check if T is parameterized type of template U
    */
    template <template <typename...> typename U, typename... Types>
    struct is_parameterized_template <U<Types...>, U>
    {
        static constexpr bool value = true;
    };

    template <typename T, template <typename...> typename U>
    inline constexpr bool is_parameterized_template_v = is_parameterized_template<T, U>::value;

    /*
    Check if T is parameterized type of template U
    */
    template <typename T, template <template <typename...> typename...> typename U>
    struct is_parameterized_template_template
    {
        static constexpr bool value = false;
    };

    /*
    Check if T is parameterized type of template U
    */
    template <template <template <typename...> typename...> typename U, template <typename...> typename... Types>
    struct is_parameterized_template_template <U<Types...>, U>
    {
        static constexpr bool value = true;
    };

    template <typename T, template <template <typename...> typename...> typename U>
    inline constexpr bool is_parameterized_template_template_v = is_parameterized_template_template<T, U>::value;

    /*
    Get information about the template
    */
    template <typename T>
    struct template_traits {}; // T is not a parametized template

    /*
    Get information about the template
    */
    template <template <typename...> typename U, typename... Types>
    struct template_traits <U<Types...>>
    {
        template <typename... Ts>
        using other_parameterized_type = U<Ts...>;

        template <size_t I>
        using argument = typename std::tuple_element<I, std::tuple<Types...>>::type;

        static constexpr size_t size = sizeof...(Types);
    };
}

#endif