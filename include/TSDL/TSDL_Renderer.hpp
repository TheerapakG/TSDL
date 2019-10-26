#ifndef TSDL_RENDERER_
#define TSDL_RENDERER_

namespace TSDL
{
    class TSDL_Renderer;
}

#include <SDL.h>
#include "TSDL_Utility.hpp"
#include "TSDL_SDLmask.hpp"
#include "TSDL_Window.hpp"

namespace TSDL
{
    class _TSDL_GET_MASK_TYPE(Texture);
    using _SDL_Texture = _TSDL_GET_MASK_TYPE(Texture);

    class TSDL_Renderer
    {
        private:
        SDL_Renderer* _internal_ptr;

        public:
        TSDL_Renderer(TSDL_Window window);
        TSDL_Renderer(TSDL_Window window, Uint32 flags);
        ~TSDL_Renderer();

        operator SDL_Renderer*() const;

        int render_color(const color_rgba& c);
        int render_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

        int viewport(const rect& rect);
        int viewport(int x, int y, int w, int h);

        int clear();

        int copy_from(_SDL_Texture texture, const rect& srcrect, const rect& dstrect);
        /*
        * like copy_from(texture, srcrect, dstrct) but after copy, optionally rotating it by angle around the given 
        * center and also optionally flipping it.
        */
        int copy_from(_SDL_Texture texture, 
                      const rect& srcrect, 
                      const rect& dstrect, 
                      const double angle, 
                      const point_2d& center,
                      const SDL_RendererFlip flip);

        int fill_rect(const rect& rect);
        int fill_rect(int x, int y, int w, int h);

        int draw_rect(const rect& rect);
        int draw_rect(int x, int y, int w, int h);

        int draw_line(int x1, int y1, int x2, int y2);

        int draw_point(int x, int y);

        int update();
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Renderer)
}

#include "TSDL_Texture.hpp"

#endif