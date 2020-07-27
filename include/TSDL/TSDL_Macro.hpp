//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_MACRO_
#define TSDL_MACRO_

#if __has_cpp_attribute(likely)
#define _TSDL_LIKELY(x) (x) [[likely]]
#else
#define _TSDL_LIKELY(x) (x)
#endif
#if __has_cpp_attribute(unlikely)
#define _TSDL_UNLIKELY(x) (x) [[unlikely]]
#else
#define _TSDL_UNLIKELY(x) (x)
#endif

#define TSDL_DECLARE_CONSTRUCT(TSDL_NAME)                 \
TSDL_##TSDL_NAME(const TSDL_##TSDL_NAME& other) = delete; \
TSDL_##TSDL_NAME(TSDL_##TSDL_NAME&& other);               \
TSDL_##TSDL_NAME& operator=(TSDL_##TSDL_NAME&& other);    \

#define TSDL_DEFINE_CONSTRUCT(NAMESPACE, TSDL_NAME)                                               \
NAMESPACE::TSDL_##TSDL_NAME::TSDL_##TSDL_NAME(TSDL_##TSDL_NAME&& other) :                         \
    _internal_ptr(other._internal_ptr), _destroy(true)                                            \
{                                                                                                 \
    other._destroy = false;                                                                       \
}                                                                                                 \
                                                                                                  \
NAMESPACE::TSDL_##TSDL_NAME& NAMESPACE::TSDL_##TSDL_NAME::operator=(TSDL_##TSDL_NAME&& other)     \
{                                                                                                 \
    _internal_ptr = other._internal_ptr;                                                          \
    _destroy = other._destroy;                                                                    \
    other._destroy = false;                                                                       \
    return *this;                                                                                 \
}                                                                                                 \

#endif