#ifndef TSDL_MAP_DEFINE_
#define TSDL_MAP_DEFINE_

#include <type_traits>

#define _TSDL_MAP_DEFINE_SIMPLE(TSDL_NAME, SDL_NAME)                          \
template <typename ...Args>                                                   \
std::invoke_result_t<decltype(TSDL_NAME), Args...> _##TSDL_NAME(Args... args) \
{                                                                             \
    return SDL_NAME(args...);                                                 \
}                                                                             \


#endif