#ifndef _TGFX_COMMON_TRAITS
#define _TGFX_COMMON_TRAITS

#include "TGFX/macros.hpp"

#include <type_traits>

TGFX_SUBSPACE_BEGIN(traits)

template <typename T, template <typename...> typename Template>
struct is_specialization : public std::false_type {};

template <template <typename...> typename Template, typename... Args>
struct is_specialization<Template<Args...>, Template> : public std::true_type {};

template <typename T, template <typename...> typename Template>
static constexpr bool is_specialization_v = is_specialization<T, Template>::value;

TGFX_SUBSPACE_END

#endif
