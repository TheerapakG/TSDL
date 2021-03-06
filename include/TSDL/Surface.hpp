//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_SURFACE_
#define TSDL_SURFACE_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Utility.hpp"
#include "Buffer.hpp"
#include "Macro.hpp"

#ifdef TSDL_EXPOSE_PYBIND11
#include "PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Surface)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Surface)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Surface)
    _PY_GET_CONTEXTMANAGER(Surface)<const std::_TSDL_U8(string)>, 
    _PY_GET_CONTEXTMANAGER(Surface)<TSDL::Buffer>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    enum class TTF_Rendermethod
    {
        Solid,
        Shaded,
        Blended,
    };

    class Font;

    class Surface
    {
        private:
        SDL_Surface* _internal_ptr = nullptr;
        bool _destroy;

        int _fill_rect(const rect* rect, Uint32 color);

        public:
        using SDL_Type = SDL_Surface;

        _PY_DECLARE_TYPEERASE_OWNER(Surface)        

        TSDL_DECLARE_CONSTRUCT(Surface)

        Surface(SDL_Surface* ptr);
        Surface(SDL_Surface* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Surface(int width, int height);
        Surface(int width, int height, int depth);
        Surface(int width, int height, Uint32 format);
        Surface(int width, int height, int depth, Uint32 format);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Surface(const std::_TSDL_U8(string)& file);
        Surface(Buffer& buffer);
        Surface(const void* mem, size_t size);

        #ifdef TSDL_USE_TTF
        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Surface(const std::_TSDL_U8(string)& text, Font& font, Uint8 r, Uint8 g, Uint8 b, TTF_Rendermethod m);
        Surface(const std::_TSDL_U8(string)& text, Font& font, const color_rgb& c, TTF_Rendermethod m);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Surface(const std::_TSDL_U8(string)& text, Font& font, Uint8 r, Uint8 g, Uint8 b, Uint8 a, TTF_Rendermethod m);
        Surface(const std::_TSDL_U8(string)& text, Font& font, const color_rgba& c, TTF_Rendermethod m);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Surface(const std::_TSDL_U8(string)& text, Font& font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 br, Uint8 bg, Uint8 bb);
        Surface(const std::_TSDL_U8(string)& text, Font& font, const color_rgb& fc, const color_rgb& bc);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Surface(const std::_TSDL_U8(string)& text, Font& font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 fa, Uint8 br, Uint8 bg, Uint8 bb, Uint8 ba);
        Surface(const std::_TSDL_U8(string)& text, Font& font, const color_rgba& fc, const color_rgba& bc);
        #endif

        ~Surface();

        operator SDL_Surface*() const;

        /*
        the format of the pixels stored in the surface; see SDL_PixelFormat for details (read-only)
        */
        const SDL_PixelFormat* format() const;

        point_2d size() const;

        /*
        Use this function to perform a fast surface copy to a destination surface.

        Remarks:
        The width and height in srcrect determine the size of the copied rectangle.
        Only the position is used in the dstrect (the width and height are ignored).
        Blits with negative dstrect coordinates will be clipped properly.
        */
        int copy_from(Surface& src, const rect& srcrect, rect& dstrect);

        /*
        Use this function to perform a scaled surface copy to a destination surface.
        */
        int scale_from(Surface& src, const rect& srcrect, rect& dstrect);

        int fill(Uint32 color);
        int fill_rect(const rect& rect, Uint32 color);

        int fill(Uint8 r, Uint8 g, Uint8 b);
        int fill_rect(const rect& rect, Uint8 r, Uint8 g, Uint8 b);
        int fill(const color_rgb& c);
        int fill_rect(const rect& rect, const color_rgb& c);

        int color_key(bool flag, Uint32 key);

        int color_key(bool flag, Uint8 r, Uint8 g, Uint8 b);
        int color_key(bool flag, const color_rgb& c);

        Uint32 color_key_Uint32();
        color_rgb color_key();

        bool has_color_key();

        Uint32 map_rgb(Uint8 r, Uint8 g, Uint8 b) const;
        Uint32 map_rgb(const color_rgb& c) const;

        Surface converted_surface(SDL_PixelFormat* fmt, Uint32 flags) const;

        void convert_surface(SDL_PixelFormat* fmt, Uint32 flags);
    };
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Surface)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Surface)
}

void _tsdl_surface_py(const py::module& m);

#endif

#endif