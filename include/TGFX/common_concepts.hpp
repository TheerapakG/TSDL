#ifndef _TGFX_COMMON_CONCEPTS
#define _TGFX_COMMON_CONCEPTS

#include "TGFX/macros.hpp"
#include "TGFX/common_traits.hpp"

#include <concepts>
#include <ranges>

TGFX_SUBSPACE_BEGIN(concepts)

template <typename MaybeRef, typename T>
concept cvref_type_of = std::same_as<std::remove_cvref_t<MaybeRef>, T>;

template <typename T, template <typename...> typename Template>
concept specialization_of = traits::is_specialization_v<T, Template>;

TGFX_SUBSPACE_END

TGFX_SUBSPACE_BEGIN(concepts::ranges)

template <typename C, typename T>
concept input_ranges_of = requires {
  requires std::ranges::input_range<C>;
  requires std::same_as<
    std::iter_value_t<
      std::ranges::iterator_t<C>
    >,
    T
  >;
};

template <typename C, typename T>
concept input_ranges_convertible_to = requires {
  requires std::ranges::input_range<C>;
  requires std::convertible_to<
    std::iter_reference_t<
      std::ranges::iterator_t<C>
    >,
    T
  >;
};

TGFX_SUBSPACE_END

#endif
