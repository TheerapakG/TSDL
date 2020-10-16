//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

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