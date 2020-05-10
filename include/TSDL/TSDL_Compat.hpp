//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_COMPAT_
#define TSDL_COMPAT_

#include <string>
#ifdef __cpp_char8_t
#ifdef __cpp_lib_char8_t
#define _TSDL_U8(x) u8##x
#define _TSDL_char char8_t
#define _TSDL_u8 u8
#else
#error compiling with char8_t support requires standard library that also support char8_t
#endif
#else
#define _TSDL_U8(x) x
#define _TSDL_char char
#define _TSDL_u8
#endif

#endif
