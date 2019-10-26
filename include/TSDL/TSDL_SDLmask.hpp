#ifndef TSDL_SDLMASK_
#define TSDL_SDLMASK_

#define _TSDL_EXPAND_DEFINE_MASK(BASE, TSDL_NAME)         \
                                                          \
class TSDL_MASK_##TSDL_NAME                               \
{                                                         \
    private:                                              \
    BASE##_##TSDL_NAME* _internal_ptr;                    \
                                                          \
    public:                                               \
    TSDL_MASK_##TSDL_NAME(BASE##_##TSDL_NAME* ptr);       \
    TSDL_MASK_##TSDL_NAME(TSDL_##TSDL_NAME obj);          \
                                                          \
    operator BASE##_##TSDL_NAME*() const;                 \
};                                                        \

#define _TSDL_EXPAND_DEFINE_MASK_TYPE(TSDL_NAME) _TSDL_EXPAND_DEFINE_MASK(SDL, TSDL_NAME)
#define _TSDL_EXPAND_DEFINE_MASK_MIX(TSDL_NAME) _TSDL_EXPAND_DEFINE_MASK(Mix, TSDL_NAME)

#define _TSDL_EXPAND_DECLARE_MASK(BASE, NAMESPACE, TSDL_NAME)                                               \
                                                                                                            \
NAMESPACE::TSDL_MASK_##TSDL_NAME::TSDL_MASK_##TSDL_NAME(BASE##_##TSDL_NAME* ptr): _internal_ptr(ptr) {}     \
NAMESPACE::TSDL_MASK_##TSDL_NAME::TSDL_MASK_##TSDL_NAME(TSDL_##TSDL_NAME obj): _internal_ptr(obj) {}        \
NAMESPACE::TSDL_MASK_##TSDL_NAME::operator BASE##_##TSDL_NAME*() const { return _internal_ptr; }            \

#define _TSDL_EXPAND_DECLARE_MASK_TYPE(NAMESPACE, TSDL_NAME) _TSDL_EXPAND_DECLARE_MASK(SDL, NAMESPACE, TSDL_NAME)
#define _TSDL_EXPAND_DECLARE_MASK_MIX(NAMESPACE, TSDL_NAME) _TSDL_EXPAND_DECLARE_MASK(Mix, NAMESPACE, TSDL_NAME)

#define _TSDL_GET_MASK_TYPE(TSDL_NAME) TSDL_MASK_##TSDL_NAME


#endif