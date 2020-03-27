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
    using optional_reference = std::optional<std::reference_wrapper<T>>;

    template <typename T>
    std::optional<std::reference_wrapper<std::remove_reference_t<T>>> make_optional_ref(T&& ref)
    {
        return ref;
    }

    template <typename T>
    std::optional<std::reference_wrapper<std::remove_reference_t<T>>> make_optional_ref()
    {
        return std::optional<std::reference_wrapper<T>>();
    }

    template <typename T>
    T& get_ref(const optional_reference<T>& stdref)
    {
        return stdref.value().get();
    }

    template <typename T>
    using optional_const_reference = std::optional<const std::reference_wrapper<T>>;

    template <typename T>
    std::optional<const std::reference_wrapper<std::remove_reference_t<T>>> make_optional_const_ref(T&& ref)
    {
        return ref;
    }

    template <typename T>
    std::optional<const std::reference_wrapper<std::remove_reference_t<T>>> make_optional_const_ref()
    {
        return std::optional<const std::reference_wrapper<T>>();
    }

    template <typename T>
    T& get_ref(const optional_const_reference<T>& stdref)
    {
        return stdref.value().get();
    }

    template <typename T>
    using transformer = std::function<T(const T&)>;
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
}

#endif