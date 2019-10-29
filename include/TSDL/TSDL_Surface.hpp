#ifndef TSDL_SURFACE_
#define TSDL_SURFACE_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "TSDL_Utility.hpp"
#include "TSDL_SDLmask.hpp"
#include <string>

namespace TSDL
{
    class _TSDL_GET_MASK_TYPE(Surface);
    using _SDL_Surface = _TSDL_GET_MASK_TYPE(Surface);

    enum class TTF_Rendermethod
    {
        Solid,
        Shaded,
        Blended,
    };

    class TSDL_Surface
    {
        private:
        SDL_Surface* _internal_ptr;
        bool _destroy;

        int _fill_rect(const rect* rect, Uint32 color);

        public:
        TSDL_Surface(SDL_Surface* ptr);

        TSDL_Surface(SDL_Surface* ptr, bool handle_destroy);

        TSDL_Surface(const std::string& file);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b, TTF_Rendermethod m);
        TSDL_Surface(const std::string& text, TTF_Font* font, const color_rgb& c, TTF_Rendermethod m);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b, Uint8 a, TTF_Rendermethod m);
        TSDL_Surface(const std::string& text, TTF_Font* font, const color_rgba& c, TTF_Rendermethod m);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 br, Uint8 bg, Uint8 bb);
        TSDL_Surface(const std::string& text, TTF_Font* font, const color_rgb& fc, const color_rgb& bc);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 fa, Uint8 br, Uint8 bg, Uint8 bb, Uint8 ba);
        TSDL_Surface(const std::string& text, TTF_Font* font, const color_rgba& fc, const color_rgba& bc);

        ~TSDL_Surface();

        operator SDL_Surface*() const;

        /*
        the format of the pixels stored in the surface; see SDL_PixelFormat for details (read-only)
        */
        const SDL_PixelFormat* format() const;

        /*
        Use this function to perform a fast surface copy to a destination surface.

        Remarks:
        The width and height in srcrect determine the size of the copied rectangle.
        Only the position is used in the dstrect (the width and height are ignored).
        Blits with negative dstrect coordinates will be clipped properly.
        */
        int copy_from(_SDL_Surface src, const rect& srcrect, rect& dstrect);

        /*
        Use this function to perform a scaled surface copy to a destination surface.
        */
        int scale_from(_SDL_Surface src, const rect& srcrect, rect& dstrect);

        int fill(Uint32 color);
        int fill_rect(const rect& rect, Uint32 color);

        int fill(Uint8 r, Uint8 g, Uint8 b);
        int fill_rect(const rect& rect, Uint8 r, Uint8 g, Uint8 b);
        int fill(const color_rgb& c);
        int fill_rect(const rect& rect, const color_rgb& c);

        int color_key(bool flag, Uint32 key);

        int color_key(bool flag, Uint8 r, Uint8 g, Uint8 b);
        int color_key(bool flag, const color_rgb& c);

        Uint32 map_rgb(Uint8 r, Uint8 g, Uint8 b) const;
        Uint32 map_rgb(const color_rgb& c) const;

        TSDL_Surface converted_surface(const SDL_PixelFormat* fmt, Uint32 flags) const;

        void convert_surface(const SDL_PixelFormat* fmt, Uint32 flags);
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Surface)
}

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Surface)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Surface)
    _PY_GET_CONTEXTMANAGER(Surface)<const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Surface)
}

void _tsdl_surface_py(const py::module& m);

#endif

#endif