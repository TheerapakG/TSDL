#include "TSDL/TSDL_Texture.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include "TSDL/TSDL_Surface.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

TSDL_DEFINE_CONSTRUCT(TSDL, Texture)

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, TSDL::_SDL_Surface surface)
{
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, surface);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, const TSDL::point_2d& size, int access)
{
    SDL_Texture* _t_internal_ptr = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, size.x, size.y);
    if (_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, const std::string& file): 
    TSDL_Texture(renderer, TSDL::TSDL_Surface(file)) {}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, TSDL_Buffer& buffer): 
    TSDL_Texture(renderer, TSDL::TSDL_Surface(buffer)) {}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, const void* mem, size_t size): 
    TSDL_Texture(renderer, TSDL::TSDL_Surface(mem, size)) {}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, TSDL::_SDL_Surface surface, Uint8 r, Uint8 g, Uint8 b)
{
    TSDL::TSDL_Surface _t_surface(surface);
    bool _surface_has_key = _t_surface.has_color_key();
    Uint32 _after_key = _t_surface.map_rgb(r, g, b);
    Uint32 _before_key = _surface_has_key? _t_surface.color_key_Uint32():_after_key;

    int _t = _t_surface.color_key(true, _after_key);
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

    _t = _t_surface.color_key(_surface_has_key, _before_key);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set texture surface color key! SDL Error: " + std::string(SDL_GetError()));
        return;
    }

    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, const std::string& file, Uint8 r, Uint8 g, Uint8 b): 
    TSDL_Texture(renderer, TSDL::TSDL_Surface(file), r, g, b) {}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, TSDL_Buffer& buffer, Uint8 r, Uint8 g, Uint8 b): 
    TSDL_Texture(renderer, TSDL::TSDL_Surface(buffer), r, g, b) {}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::_SDL_Renderer& renderer, const void* mem, size_t size, Uint8 r, Uint8 g, Uint8 b): 
    TSDL_Texture(renderer, TSDL::TSDL_Surface(mem, size), r, g, b) {}

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
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, const std::string>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, TSDL::TSDL_Buffer, Uint8, Uint8, Uint8>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, TSDL::TSDL_Buffer, Uint8, Uint8, Uint8>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, TSDL::_SDL_Surface>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Texture)<TSDL::_SDL_Renderer, TSDL::_SDL_Surface, Uint8, Uint8, Uint8>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Texture, enter_ctx), py::return_value_policy::reference)
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