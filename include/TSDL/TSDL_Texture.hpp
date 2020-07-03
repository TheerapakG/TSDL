//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_TEXTURE_
#define TSDL_TEXTURE_

#include <SDL2/SDL.h>
#include "TSDL_Utility.hpp"
#include "TSDL_Buffer.hpp"
#include "TSDL_Macro.hpp"

namespace TSDL
{
    class TSDL_Surface;

    class TSDL_Renderer;

    class TSDL_Texture;
}

#ifdef TSDL_EXPOSE_PYBIND11
#include "TSDL_PY_TypeErase.hpp"
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Texture)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Texture)
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::TSDL_Renderer, TSDL::TSDL_Surface>, 
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::TSDL_Renderer, const std::_TSDL_U8(string)>, 
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::TSDL_Renderer, TSDL::TSDL_Buffer>, 
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::TSDL_Renderer, TSDL::TSDL_Surface, Uint8, Uint8, Uint8>,
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::TSDL_Renderer, const std::_TSDL_U8(string), Uint8, Uint8, Uint8>, 
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::TSDL_Renderer, TSDL::TSDL_Buffer, Uint8, Uint8, Uint8>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class TSDL_Texture
    {
        private:
        SDL_Texture* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = SDL_Texture;

        _PY_DECLARE_TYPEERASE_OWNER(Texture)

        TSDL_DECLARE_CONSTRUCT(Texture)

        TSDL_Texture(SDL_Texture* ptr);
        TSDL_Texture(SDL_Texture* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Texture(TSDL_Renderer& renderer, TSDL_Surface& surface);
        TSDL_Texture(TSDL_Renderer& renderer, const point_2d& size, int access = SDL_TEXTUREACCESS_TARGET, int format = SDL_PIXELFORMAT_RGBA8888);
        TSDL_Texture(TSDL_Renderer& renderer, const std::_TSDL_U8(string)& file);        
        TSDL_Texture(TSDL_Renderer& renderer, TSDL_Buffer& buffer);
        TSDL_Texture(TSDL_Renderer& renderer, const void* mem, size_t size);

        /*
        Create texture from file with color r, g, b as transparent color

        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Texture(TSDL_Renderer& renderer, TSDL_Surface& surface, Uint8 r, Uint8 g, Uint8 b);
        TSDL_Texture(TSDL_Renderer& renderer, const std::_TSDL_U8(string)& file, Uint8 r, Uint8 g, Uint8 b);
        TSDL_Texture(TSDL_Renderer& renderer, TSDL_Buffer& buffer, Uint8 r, Uint8 g, Uint8 b);
        TSDL_Texture(TSDL_Renderer& renderer, const void* mem, size_t size, Uint8 r, Uint8 g, Uint8 b);

        ~TSDL_Texture();

        operator SDL_Texture*() const;

        /*
        raw format of the texture; the actual format may differ, but pixel transfers will use this format
        */
        SDL_PixelFormatEnum format() const;

        point_2d size() const;

        int blend_mode(SDL_BlendMode mode);
        SDL_BlendMode blend_mode();

        /*
        When this texture is rendered, during the copy operation alpha is used to calculate each pixel based on blending mode.
        */
        int alpha_multiplier(Uint8 a);
        Uint8 alpha_multiplier();

        /*
        When this texture is rendered, during the copy operation each source color channel value is calculated using this:
            srcC = srcC * (color / 255)
        */
        int color_multiplier(Uint8 r, Uint8 g, Uint8 b);
        int color_multiplier(const color_rgb& c);
        color_rgb color_multiplier();
    };
}

#include "TSDL/TSDL_Renderer.hpp"
#include "TSDL/TSDL_Surface.hpp"

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Texture)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Texture)
}

void _tsdl_texture_py(const py::module& m);

#endif

#endif