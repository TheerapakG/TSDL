//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/TSDL_Surface.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include "TSDL/TSDL_Font.hpp"

TSDL_DEFINE_CONSTRUCT(TSDL, Surface)

TSDL::TSDL_Surface::TSDL_Surface(SDL_Surface* ptr): TSDL_Surface(ptr, false) {}

TSDL::TSDL_Surface::TSDL_Surface(SDL_Surface* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Surface::TSDL_Surface(int width, int height): TSDL_Surface(width, height, 32) {}

TSDL::TSDL_Surface::TSDL_Surface(int width, int height, int depth): _destroy(true)
{
    Uint32 rmask, gmask, bmask, amask;
    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    SDL_Surface* _t_internal_ptr = SDL_CreateRGBSurface(0, width, height, depth, rmask, gmask, bmask, amask);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(int width, int height, Uint32 format): TSDL_Surface(width, height, 32, format) {}

TSDL::TSDL_Surface::TSDL_Surface(int width, int height, int depth, Uint32 format): _destroy(true)
{
    SDL_Surface* _t_internal_ptr = SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format);;
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& file): _destroy(true)
{
    SDL_Surface* _t_internal_ptr = IMG_Load(reinterpret_cast<const char*>(file.c_str()));
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

SDL_Surface* _create_surface_from_buffer(TSDL::TSDL_Buffer& buffer)
{
    SDL_Surface* _t_ptr = IMG_Load_RW(buffer, 0); // We free buffer by utilizing the scope
    if(_t_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return nullptr;
    }
    return _t_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(TSDL_Buffer& buffer): _destroy(true)
{
    SDL_Surface* _t_internal_ptr = _create_surface_from_buffer(buffer);
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(const void* mem, size_t size): _destroy(true)
{
    TSDL::TSDL_Buffer _t_buffer = TSDL::TSDL_Buffer(mem, size);
    SDL_Surface* _t_internal_ptr = _create_surface_from_buffer(_t_buffer);
    _internal_ptr = _t_internal_ptr;
}

#ifdef TSDL_USE_TTF

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, Uint8 r, Uint8 g, Uint8 b, TSDL::TTF_Rendermethod m):
    TSDL::TSDL_Surface(text, font, r, g, b, 255, m){}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, const TSDL::color_rgb& c, TSDL::TTF_Rendermethod m):
    TSDL::TSDL_Surface(text, font, c.r, c.g, c.b, m){}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, Uint8 r, Uint8 g, Uint8 b, Uint8 a, TSDL::TTF_Rendermethod m): _destroy(true)
{
    SDL_Surface* _t_internal_ptr = nullptr;
    switch (m)
    {
    case TSDL::TTF_Rendermethod::Solid:
        _t_internal_ptr = TTF_RenderUTF8_Solid(font, reinterpret_cast<const char*>(text.c_str()), {r, g, b, a});
        break;

    case TSDL::TTF_Rendermethod::Shaded:
        // assume white bg with 0 as alpha
        _t_internal_ptr = TTF_RenderUTF8_Shaded(font, reinterpret_cast<const char*>(text.c_str()), {r, g, b, a}, {255, 255, 255, 0});
        break;

    case TSDL::TTF_Rendermethod::Blended:
        _t_internal_ptr = TTF_RenderUTF8_Blended(font, reinterpret_cast<const char*>(text.c_str()), {r, g, b, a});
        break;
    
    default:
        TSDL::safe_throw<std::runtime_error>("Invalid Rendermethod specified!");
        _internal_ptr = nullptr;
        return;
    }
    
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Surface could not be created! SDL_Error: " + std::string(TTF_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, const TSDL::color_rgba& c, TSDL::TTF_Rendermethod m):
    TSDL::TSDL_Surface(text, font, c.r, c.g, c.b, c.a, m){}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 br, Uint8 bg, Uint8 bb):
    TSDL::TSDL_Surface(text, font, fr, fg, fb, 255, br, bg, bb, 0){}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, const TSDL::color_rgb& fc, const TSDL::color_rgb& bc):
    TSDL::TSDL_Surface(text, font, fc.r, fc.g, fc.b, bc.r, bc.g, bc.b){}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 fa, Uint8 br, Uint8 bg, Uint8 bb, Uint8 ba): 
    _destroy(true)
{
    SDL_Surface* _t_internal_ptr = TTF_RenderUTF8_Shaded(font, reinterpret_cast<const char*>(text.c_str()), {fr, fg, fb, fa}, {br, bg, bb, ba});
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(const std::_TSDL_U8(string)& text, TSDL_Font& font, const TSDL::color_rgba& fc, const TSDL::color_rgba& bc):
    TSDL::TSDL_Surface(text, font, fc.r, fc.g, fc.b, fc.a, bc.r, bc.g, bc.b, bc.a){}

#endif

TSDL::TSDL_Surface::~TSDL_Surface()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_FreeSurface(_internal_ptr);
}

TSDL::TSDL_Surface::operator SDL_Surface*() const
{
    return _internal_ptr;
}

const SDL_PixelFormat* TSDL::TSDL_Surface::format() const
{
    return _internal_ptr->format;
}

::TSDL::point_2d TSDL::TSDL_Surface::size() const
{
    return {_internal_ptr->w, _internal_ptr->h};
}

int TSDL::TSDL_Surface::copy_from(TSDL_Surface& src, const rect& srcrect, rect& dstrect)
{
    int _t = SDL_BlitSurface(src, &srcrect, _internal_ptr, &dstrect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot copy from surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::scale_from(TSDL_Surface& src, const rect& srcrect, rect& dstrect)
{
    int _t = SDL_BlitScaled(src, &srcrect, _internal_ptr, &dstrect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot scaled copy from surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::_fill_rect(const rect* rect, Uint32 color)
{
    int _t = SDL_FillRect(_internal_ptr, rect, color);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot copy from surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::fill(Uint32 color)
{
    return this->_fill_rect(NULL, color);
}

int TSDL::TSDL_Surface::fill_rect(const rect& rect, Uint32 color)
{
    return this->_fill_rect(&rect, color);
}

int TSDL::TSDL_Surface::fill(Uint8 r, Uint8 g, Uint8 b)
{
    return this->fill(this->map_rgb(r, g, b));
}

int TSDL::TSDL_Surface::fill_rect(const rect& rect, Uint8 r, Uint8 g, Uint8 b)
{
    return this->fill_rect(rect, this->map_rgb(r, g, b));
}

int TSDL::TSDL_Surface::fill(const TSDL::color_rgb& c)
{
    return this->fill(c.r, c.g, c.b);
}

int TSDL::TSDL_Surface::fill_rect(const rect& rect, const TSDL::color_rgb& c)
{
    return this->fill_rect(rect, c.r, c.g, c.b);
}

int TSDL::TSDL_Surface::color_key(bool flag, Uint32 key)
{
    int _t = SDL_SetColorKey(_internal_ptr, flag, key);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set surface color key! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::color_key(bool flag, Uint8 r, Uint8 g, Uint8 b)
{
    return this->color_key(flag, this->map_rgb(r, g, b));
}

int TSDL::TSDL_Surface::color_key(bool flag, const TSDL::color_rgb& c)
{
    return this->color_key(flag, c.r, c.g, c.b);
}

Uint32 TSDL::TSDL_Surface::color_key_Uint32()
{
    Uint32 _ret;
    int _t = SDL_GetColorKey(_internal_ptr, &_ret);
    if(_t == -1)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get surface color key! Color Key is not enabled for this suface!");
        // TODO: noexcept signify error
    }
    else if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get surface color key! SDL Error: " + std::string(SDL_GetError()));
        // TODO: noexcept signify error
    }
    return _ret;
}

TSDL::color_rgb TSDL::TSDL_Surface::color_key()
{
    Uint8 r, g, b;
    Uint32 _col = this->color_key_Uint32(); // TODO: check when noexcept signify error
    SDL_GetRGB(_col, _internal_ptr->format, &r, &g, &b);
    return TSDL::color_rgb(r, g, b);
}

bool TSDL::TSDL_Surface::has_color_key()
{
    return static_cast<bool>(SDL_HasColorKey(_internal_ptr));
}

Uint32 TSDL::TSDL_Surface::map_rgb(Uint8 r, Uint8 g, Uint8 b) const
{
    return SDL_MapRGB(_internal_ptr->format, r, g, b);
}

Uint32 TSDL::TSDL_Surface::map_rgb(const TSDL::color_rgb& c) const
{
    return this->map_rgb(c.r, c.g, c.b);
}

SDL_Surface* _safe_convert_surface(SDL_Surface* src, SDL_PixelFormat* fmt, Uint32 flags)
{
    SDL_Surface* _t = SDL_ConvertSurface(src, fmt, flags);
    if(_t == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot convert surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::TSDL_Surface TSDL::TSDL_Surface::converted_surface(SDL_PixelFormat* fmt, Uint32 flags) const
{
    return TSDL::TSDL_Surface(_safe_convert_surface(_internal_ptr, fmt, flags), true);
}

void TSDL::TSDL_Surface::convert_surface(SDL_PixelFormat* fmt, Uint32 flags)
{
    SDL_Surface* _t_internal_ptr = _safe_convert_surface(_internal_ptr, fmt, flags);
    if(_destroy) SDL_FreeSurface(_internal_ptr);
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Surface)

void _tsdl_surface_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Surface)>(m, "Surface")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Surface)<const std::_TSDL_U8(string)>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Surface, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Surface, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Surface, exit_ctx));
    py::class_<TSDL::TSDL_Surface>(m, "_Surface")
        .def("format", &TSDL::TSDL_Surface::format)
        .def("copy_from", &TSDL::TSDL_Surface::copy_from)
        .def("scale_from", &TSDL::TSDL_Surface::scale_from)
        .def("fill_rect", py::overload_cast<const TSDL::rect&, Uint32>(&TSDL::TSDL_Surface::fill_rect), 
            py::arg("rect"),
            py::arg("color")
        )
        .def("fill", py::overload_cast<Uint32>(&TSDL::TSDL_Surface::fill), 
            py::arg("color")
        )
        .def("fill_rect", py::overload_cast<const TSDL::rect&, Uint8, Uint8, Uint8>(&TSDL::TSDL_Surface::fill_rect), 
            py::arg("rect"),
            py::arg("r"),
            py::arg("g"),
            py::arg("b")
        )
        .def("fill", py::overload_cast<Uint8, Uint8, Uint8>(&TSDL::TSDL_Surface::fill), 
            py::arg("r"),
            py::arg("g"),
            py::arg("b")
        )
        .def("color_key", py::overload_cast<bool, Uint32>(&TSDL::TSDL_Surface::color_key))
        .def("color_key", py::overload_cast<bool, Uint8, Uint8, Uint8>(&TSDL::TSDL_Surface::color_key))
        .def("color_key_Uint32", &TSDL::TSDL_Surface::color_key_Uint32)
        .def("has_color_key", &TSDL::TSDL_Surface::has_color_key)
        .def("map_rgb", py::overload_cast<Uint8, Uint8, Uint8>(&TSDL::TSDL_Surface::map_rgb, py::const_))
        .def("converted_surface", &TSDL::TSDL_Surface::converted_surface)
        .def("convert_surface", &TSDL::TSDL_Surface::convert_surface);
}

#endif