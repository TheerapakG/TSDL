#ifndef TSDL_UTILITIES_OPTIONAL_WITH_BASE
#define TSDL_UTILITIES_OPTIONAL_WITH_BASE

#include "TSDL/TSDL_Meta.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include <optional>
#include <tuple>
#include <functional>

namespace TSDL::util
{
    template <typename... Types>
    class optional_with_base
    {
        private:
        using _Tuple_T = typename get_tuple_t<std::reference_wrapper<Types...>>;
        std::optional<_Tuple_T> _references_tuple;

        public:
        constexpr optional_with_base() noexcept {}
        constexpr optional_with_base(std::nullopt_t) noexcept {}

        template <typename T, std::enable_if_t<_and_v<std::is_base_of_v<Types, T>...>>>
        constexpr optional_with_base(const T& value): _references_tuple(std::in_place, static_cast<const Types&>(value)...) {}
        template <typename T, std::enable_if_t<_and_v<std::is_base_of_v<Types, T>...>>>
        constexpr optional_with_base(T&& value): _references_tuple(std::in_place, std::forward<Types>(value)...) {}

        constexpr optional_with_base& operator=(const optional_with_base& other)
        {
            _references_tuple = other._references_tuple;
            return *this;
        }

        constexpr optional_with_base& operator=(optional_with_base&& other) noexcept(std::is_nothrow_move_assignable_v<_Tuple_T>)
        {
            _references_tuple = std::forward<_Tuple_T>(other._references_tuple);
            return *this;
        }

        template <typename... UTypes, typename = std::enable_if<
            std::is_assignable<_Tuple_T&, const get_tuple_t<std::reference_wrapper<UTypes...>>&>
        >>
        constexpr optional_with_base& operator=(const optional_with_base<UTypes...>& other)
        {
            _references_tuple = other._references_tuple;
            return *this;
        }

        template <typename... UTypes, typename = std::enable_if<
            std::is_assignable<_Tuple_T&, get_tuple_t<std::reference_wrapper<UTypes...>>&&>
        >>
        constexpr optional_with_base& operator=(optional_with_base<UTypes...>&& other)
        {
            _references_tuple = std::forward<_Tuple_T>(other._references_tuple);
            return *this;
        }

        template <typename T, typename... _Types>
        friend constexpr T& get(optional_with_base<_Types...>& val);

        template <typename T, typename... Types>
        friend constexpr T&& get(optional_with_base<_Types...>&& val);

        template <typename T, typename... Types>
        friend constexpr const T& get(const optional_with_base<_Types...>& val);

        template <typename T, typename... Types>
        friend constexpr const T&& get(const optional_with_base<_Types...>&& val);
    };

    template <typename T, typename... _Types>
    constexpr T& get(optional_with_base<_Types...>& val)
    {
        return std::get<T>(val._references_tuple);
    }

    template <typename T, typename... _Types>
    constexpr T&& get(optional_with_base<_Types...>&& val)
    {
        return std::get<T>(val._references_tuple);
    }

    template <typename T, typename... _Types>
    constexpr const T& get(const optional_with_base<_Types...>& val)
    {
        return std::get<T>(val._references_tuple);
    }

    template <typename T, typename... _Types>
    constexpr const T&& get(const optional_with_base<_Types...>&& val)
    {
        return std::get<T>(val._references_tuple);
    }
}

#endif