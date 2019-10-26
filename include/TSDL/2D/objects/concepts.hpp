#ifndef TSDL_2D_OBJECTS_CONCEPTS_
#define TSDL_2D_OBJECTS_CONCEPTS_

#include "TSDL/TSDL_Features.hpp"

#ifndef TSDL_NO_LIB_CONCEPTS

#else
template <typename T>
concept HasGetWidth = requires (T t) {{t::get_width} -> std::invocable<>}

template <typename T>
concept HasSetWidth = requires (T t) {{t::set_width} -> std::invocable<std::unsigned_integral>}

template <typename T>
concept HasPropertyWidth = HasGetWidth<T> && HasSetWidth<T>;

template <typename T>
concept HasGetLength = requires (T t) {{t::get_length;} -> std::invocable<>}

template <typename T>
concept HasSetLength = requires (T t) {{t::set_length;} -> std::invocable<std::unsigned_integral>}

#endif

#endif