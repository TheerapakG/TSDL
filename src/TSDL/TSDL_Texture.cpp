#include "TSDL/TSDL_Texture.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include "TSDL/TSDL_Surface.hpp"
#include <SDL.h>
#include <SDL_image.h>

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer renderer, const std::string& file)
{
    TSDL::TSDL_Surface _t_surface(file);
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, _t_surface);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer renderer, const std::string& file, Uint8 r, Uint8 g, Uint8 b)
{
    TSDL::TSDL_Surface _t_surface(file);
    int _t = _t_surface.color_key(true, r, g, b);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set texture surface color key! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, _t_surface);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer renderer, TSDL::_SDL_Surface surface)
{
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, surface);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::~TSDL_Texture()
{
    if(_internal_ptr != nullptr) SDL_DestroyTexture(_internal_ptr);
}

TSDL::TSDL_Texture::operator SDL_Texture*() const
{
    return _internal_ptr;
}

int TSDL::TSDL_Texture::blend_mode(SDL_BlendMode mode)
{
    int _t = SDL_SetTextureBlendMode(_internal_ptr, mode);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set blend mode! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

SDL_BlendMode TSDL::TSDL_Texture::blend_mode()
{
    SDL_BlendMode ret;
    int _t = SDL_GetTextureBlendMode(_internal_ptr, &ret);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get blend mode! SDL Error: " + std::string(SDL_GetError()));
        // TODO: noexcept signify error
    }
    return ret;
}

int TSDL::TSDL_Texture::alpha_multiplier(Uint8 a)
{
    int _t = SDL_SetTextureAlphaMod(_internal_ptr, a);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set alpha! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

Uint8 TSDL::TSDL_Texture::alpha_multiplier()
{
    Uint8 ret;
    int _t = SDL_GetTextureAlphaMod(_internal_ptr, &ret);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get alpha! SDL Error: " + std::string(SDL_GetError()));
        // TODO: noexcept signify error
    }
    return ret;
}

int TSDL::TSDL_Texture::color_multiplier(Uint8 r, Uint8 g, Uint8 b)
{
    int _t = SDL_SetTextureColorMod(_internal_ptr, r, g, b);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set color multiplier! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Texture::color_multiplier(const color_rgb& c)
{
    return this->color_multiplier(c.r, c.g, c.b);
}

TSDL::color_rgb TSDL::TSDL_Texture::color_multiplier()
{
    SDL_Color ret;
    int _t = SDL_GetTextureColorMod(_internal_ptr, &ret.r, &ret.g, &ret.b);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get color multiplier! SDL Error: " + std::string(SDL_GetError()));
    }
    return ret;
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Texture)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Texture)

void _tsdl_texture_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Texture)>(m, "Texture")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, const std::string>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, const std::string, Uint8, Uint8, Uint8>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, TSDL::_SDL_Surface>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, exit_ctx));
    py::class_<TSDL::TSDL_Texture>(m, "_Texture")
        .def_property("color_multiplier",
            py::overload_cast<const TSDL::color_rgb&>(&TSDL::TSDL_Texture::color_multiplier),
            py::overload_cast<>(&TSDL::TSDL_Texture::color_multiplier)                                                                                                    \
        );
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Texture)>(m, "_SDL_Texture");
    py::implicitly_convertible<TSDL::TSDL_Texture, TSDL::_TSDL_GET_MASK_TYPE(Texture)>();
}

#endif