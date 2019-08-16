#ifndef TSDL_TEXTURE_
#define TSDL_TEXTURE_

namespace TSDL
{
    class TSDL_Texture;
}

#include <SDL.h>
#include "TSDL_SDLmask.hpp"
#include "TSDL_Renderer.hpp"
#include <string>

namespace TSDL
{
    class TSDL_Texture
    {
        private:
        SDL_Texture* _internal_ptr;

        public:
        TSDL_Texture(TSDL_Renderer renderer, const std::string& file);
        ~TSDL_Texture();

        operator SDL_Texture*() const;
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Texture)
}

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Texture)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Texture)
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::TSDL_Renderer, const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Texture)
}

#define _TSDL_TEXTURE_PY                                                                                              \
    py::class_<_PY::_PY_GET_TYPEERASE(Texture)>(m, "Texture")                                                         \
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::TSDL_Renderer, const std::string>())                       \
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, enter_ctx), py::return_value_policy::reference)   \
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, exit_ctx));                                        \
    py::class_<TSDL::TSDL_Texture>(m, "_Texture");                                                                    \
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Texture)>(m, "_SDL_Texture");                                                \
    py::implicitly_convertible<TSDL::TSDL_Texture, TSDL::_TSDL_GET_MASK_TYPE(Texture)>();                             \

#endif

#endif