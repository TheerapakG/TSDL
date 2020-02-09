#ifndef TSDL_META_
#define TSDL_META_

#include <type_traits>
#include <optional>
#include <functional>
#include <tuple>

namespace TSDL
{
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
    std::optional<std::reference_wrapper<T>> make_optional_ref(T&& ref)
    {
        return ref;
    }

    template <typename T>
    std::optional<std::reference_wrapper<T>> make_optional_ref()
    {
        return std::optional<std::reference_wrapper<T>>();
    }

    template <typename T>
    std::optional<const std::reference_wrapper<T>> make_optional_const_ref(T&& ref)
    {
        return ref;
    }

    template <typename T>
    std::optional<const std::reference_wrapper<T>> make_optional_const_ref()
    {
        return std::optional<const std::reference_wrapper<T>>();
    }
}

#endif