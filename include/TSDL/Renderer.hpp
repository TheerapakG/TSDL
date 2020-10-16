//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_RENDERER_
#define TSDL_RENDERER_

namespace TSDL
{
    class Texture;
    class Renderer;
}

#include <optional>
#include <SDL2/SDL.h>
#include "Meta.hpp"
#include "Utility.hpp"
#include "Macro.hpp"
#include "Window.hpp"

namespace TSDL
{
    class Renderer
    {
        private:
        SDL_Renderer* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = SDL_Renderer;

        TSDL_DECLARE_CONSTRUCT(Renderer)

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Renderer(Window& window);
        Renderer(Window& window, Uint32 flags);

        ~Renderer();

        operator SDL_Renderer*() const;

        std::optional<Texture> target();
        int target(std::optional<std::reference_wrapper<Texture>> texture);

        /*
        Size of the rendering target
        */
        point_2d render_size();

        color_rgba render_color();
        int render_color(const color_rgba& c);
        int render_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

        int viewport(const rect& rect);
        int viewport(int x, int y, int w, int h);

        int clear();
        int clear(const color_rgba& c);

        int copy_from(
            Texture& texture, 
            optional_const_reference<rect> srcrect, 
            optional_const_reference<rect> dstrect
        );
        /*
        * like copy_from(texture, srcrect, dstrct) but after copy, optionally rotating it by angle around the given 
        * center and also optionally flipping it.
        */
        int copy_from(Texture& texture, 
                      optional_const_reference<rect> srcrect,
                      optional_const_reference<rect> dstrect,
                      const double angle, 
                      optional_const_reference<point_2d> center,
                      const SDL_RendererFlip flip);

        int fill_rect(const rect& rect);
        int fill_rect(int x, int y, int w, int h);

        int fill_rect(const color_rgba& c, const rect& rect);
        int fill_rect(const color_rgba& c, int x, int y, int w, int h);

        int draw_rect(const rect& rect);
        int draw_rect(int x, int y, int w, int h);

        int draw_line(int x1, int y1, int x2, int y2);

        int draw_point(int x, int y);

        int update();
        int update(const color_rgba& c);
    };
}

#include "Texture.hpp"

#endif