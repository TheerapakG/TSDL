#ifndef TSDL_SDLMASK_
#define TSDL_SDLMASK_

#define _TSDL_EXPAND_DEFINE_MASK_TYPE(TSDL_NAME)          \
                                                          \
class TSDL_MASK_##TSDL_NAME                               \
{                                                         \
    using SDL_TYPE = typename TSDL_##TSDL_NAME::SDL_Type; \
    private:                                              \
    SDL_TYPE* _internal_ptr;                              \
                                                          \
    public:                                               \
    TSDL_MASK_##TSDL_NAME(SDL_TYPE* ptr);                 \
    TSDL_MASK_##TSDL_NAME(TSDL_##TSDL_NAME obj);          \
                                                          \
    operator SDL_TYPE*() const;                           \
};                                                        \

#define _TSDL_EXPAND_DECLARE_MASK_TYPE(NAMESPACE, TSDL_NAME)                                                                    \
                                                                                                                                \
NAMESPACE::TSDL_MASK_##TSDL_NAME::TSDL_MASK_##TSDL_NAME(NAMESPACE::TSDL_##TSDL_NAME::SDL_Type* ptr): _internal_ptr(ptr) {}      \
NAMESPACE::TSDL_MASK_##TSDL_NAME::TSDL_MASK_##TSDL_NAME(NAMESPACE::TSDL_##TSDL_NAME obj): _internal_ptr(obj) {}                 \
NAMESPACE::TSDL_MASK_##TSDL_NAME::operator NAMESPACE::TSDL_##TSDL_NAME::SDL_Type*() const { return _internal_ptr; }             \

#define _TSDL_GET_MASK_TYPE(TSDL_NAME) TSDL_MASK_##TSDL_NAME


#endif