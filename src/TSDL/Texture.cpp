//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Texture.hpp"
#include "TSDL/Utility.hpp"
#include "TSDL/Surface.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

TSDL_DEFINE_CONSTRUCT(TSDL, Texture)

TSDL::Texture::Texture(SDL_Texture* ptr): Texture(ptr, false) {}

TSDL::Texture::Texture(SDL_Texture* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::Texture::Texture(TSDL::Renderer& renderer, TSDL::Surface& surface)
{
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, surface);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::Texture::Texture(TSDL::Renderer& renderer, const TSDL::point_2d& size, int access, int format)
{
    SDL_Texture* _t_internal_ptr = SDL_CreateTexture(renderer, format, access, size.x, size.y);
    if (_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::Texture::Texture(TSDL::Renderer& renderer, const std::_TSDL_U8(string)& file): 
    Texture(renderer, TSDL::Surface(file)) {}

TSDL::Texture::Texture(TSDL::Renderer& renderer, Buffer& buffer): 
    Texture(renderer, TSDL::Surface(buffer)) {}

TSDL::Texture::Texture(TSDL::Renderer& renderer, const void* mem, size_t size): 
    Texture(renderer, TSDL::Surface(mem, size)) {}

TSDL::Texture::Texture(TSDL::Renderer& renderer, TSDL::Surface& surface, Uint8 r, Uint8 g, Uint8 b)
{
    bool _surface_has_key = surface.has_color_key();
    Uint32 _after_key = surface.map_rgb(r, g, b);
    Uint32 _before_key = _surface_has_key? surface.color_key_Uint32():_after_key;

    int _t = surface.color_key(true, _after_key);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set texture surface color key! SDL Error: " + std::string(SDL_GetError()));
        return;
    }

    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, surface);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }

    _t = surface.color_key(_surface_has_key, _before_key);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set texture surface color key! SDL Error: " + std::string(SDL_GetError()));
        return;
    }

    _internal_ptr = _t_internal_ptr;
}

TSDL::Texture::Texture(TSDL::Renderer& renderer, const std::_TSDL_U8(string)& file, Uint8 r, Uint8 g, Uint8 b): 
    Texture(renderer, TSDL::Surface(file), r, g, b) {}

TSDL::Texture::Texture(TSDL::Renderer& renderer, Buffer& buffer, Uint8 r, Uint8 g, Uint8 b): 
    Texture(renderer, TSDL::Surface(buffer), r, g, b) {}

TSDL::Texture::Texture(TSDL::Renderer& renderer, const void* mem, size_t size, Uint8 r, Uint8 g, Uint8 b): 
    Texture(renderer, TSDL::Surface(mem, size), r, g, b) {}

TSDL::Texture::~Texture()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_DestroyTexture(_internal_ptr);
}

TSDL::Texture::operator SDL_Texture*() const
{
    return _internal_ptr;
}

SDL_PixelFormatEnum TSDL::Texture::format() const
{
    Uint32 format;
    int _t = SDL_QueryTexture(_internal_ptr, &format, NULL, NULL, NULL);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get format! SDL Error: " + std::string(SDL_GetError()));
    }
    return static_cast<SDL_PixelFormatEnum>(format);
}

::TSDL::point_2d TSDL::Texture::size() const
{
    int w, h;
    int _t = SDL_QueryTexture(_internal_ptr, NULL, NULL, &w, &h);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get size! SDL Error: " + std::string(SDL_GetError()));
    }
    return {w, h};
}

int TSDL::Texture::blend_mode(SDL_BlendMode mode)
{
    int _t = SDL_SetTextureBlendMode(_internal_ptr, mode);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set blend mode! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

SDL_BlendMode TSDL::Texture::blend_mode()
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

int TSDL::Texture::alpha_multiplier(Uint8 a)
{
    int _t = SDL_SetTextureAlphaMod(_internal_ptr, a);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set alpha! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

Uint8 TSDL::Texture::alpha_multiplier()
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

int TSDL::Texture::color_multiplier(Uint8 r, Uint8 g, Uint8 b)
{
    int _t = SDL_SetTextureColorMod(_internal_ptr, r, g, b);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set color multiplier! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Texture::color_multiplier(const color_rgb& c)
{
    return this->color_multiplier(c.r, c.g, c.b);
}

TSDL::color_rgb TSDL::Texture::color_multiplier()
{
    SDL_Color ret;
    int _t = SDL_GetTextureColorMod(_internal_ptr, &ret.r, &ret.g, &ret.b);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get color multiplier! SDL Error: " + std::string(SDL_GetError()));
    }
    return ret;
}

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Texture)

void _tsdl_texture_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Texture)>(m, "Texture")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::Renderer, const std::_TSDL_U8(string)>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::Renderer, const std::_TSDL_U8(string)>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::Renderer, TSDL::Buffer, Uint8, Uint8, Uint8>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::Renderer, TSDL::Buffer, Uint8, Uint8, Uint8>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::Renderer, TSDL::Surface>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::Renderer, TSDL::Surface, Uint8, Uint8, Uint8>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, exit_ctx));
    py::class_<TSDL::Texture>(m, "_Texture")
        .def_property("color_multiplier",
            py::overload_cast<const TSDL::color_rgb&>(&TSDL::Texture::color_multiplier),
            py::overload_cast<>(&TSDL::Texture::color_multiplier)                                                                                                    \
        );
}

#endif