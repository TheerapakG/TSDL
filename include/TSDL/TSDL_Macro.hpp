#ifndef TSDL_MACRO_
#define TSDL_MACRO_

#define TSDL_DECLARE_CONSTRUCT(TSDL_NAME)                 \
TSDL_##TSDL_NAME(const TSDL_##TSDL_NAME& other) = delete; \
TSDL_##TSDL_NAME(TSDL_##TSDL_NAME&& other);               \

#define TSDL_DEFINE_CONSTRUCT(NAMESPACE, TSDL_NAME)                           \
NAMESPACE::TSDL_##TSDL_NAME::TSDL_##TSDL_NAME(TSDL_##TSDL_NAME&& other) :     \
    _internal_ptr(other._internal_ptr), _destroy(true)                        \
{                                                                             \
    other._destroy = false;                                                   \
}                                                                             \

#endif