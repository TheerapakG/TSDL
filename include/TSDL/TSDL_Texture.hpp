#ifndef TSDL_TEXTURE_
#define TSDL_TEXTURE_

namespace TSDL
{
    class TSDL_Texture;
}

#include <SDL.h>
#include "TSDL_SDLmask.hpp"
#include "TSDL_Utility.hpp"
#include <string>

namespace TSDL
{
    class _TSDL_GET_MASK_TYPE(Surface);
    using _SDL_Surface = _TSDL_GET_MASK_TYPE(Surface);

    class _TSDL_GET_MASK_TYPE(Renderer);
    using _SDL_Renderer = _TSDL_GET_MASK_TYPE(Renderer);

    class TSDL_Texture
    {
        private:
        SDL_Texture* _internal_ptr;

        public:
        TSDL_Texture(_SDL_Renderer renderer, const std::string& file);
        /*
        Create texture from file with color r, g, b as transparent color
        */
        TSDL_Texture(_SDL_Renderer renderer, const std::string& file, Uint8 r, Uint8 g, Uint8 b);
        TSDL_Texture(_SDL_Renderer renderer, _SDL_Surface surface);
        ~TSDL_Texture();

        operator SDL_Texture*() const;

        void blend_mode(SDL_BlendMode mode);
        SDL_BlendMode blend_mode();

        /*
        When this texture is rendered, during the copy operation alpha is used to calculate each pixel based on blending mode.
        */
        void alpha_multiplier(Uint8 a);
        Uint8 alpha_multiplier();

        /*
        When this texture is rendered, during the copy operation each source color channel value is calculated using this:
            srcC = srcC * (color / 255)
        */
        void color_multiplier(Uint8 r, Uint8 g, Uint8 b);
        void color_multiplier(const color_rgb& c);
        color_rgb color_multiplier();
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Texture)
}

#include "TSDL/TSDL_Renderer.hpp"
#include "TSDL/TSDL_Surface.hpp"

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Texture)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Texture)
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::_SDL_Renderer, const std::string>, 
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::_SDL_Renderer, const std::string, Uint8, Uint8, Uint8>, 
    _PY_GET_CONTEXTMANAGER(Texture)<TSDL::_SDL_Renderer, TSDL::_SDL_Surface>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Texture)
}

void _tsdl_texture_py(const py::module& m);

#endif

#endif