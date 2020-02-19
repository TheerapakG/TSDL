#ifndef TSDL_RENDERER_
#define TSDL_RENDERER_

namespace TSDL
{
    class TSDL_Renderer;
}

#include <optional>
#include <SDL2/SDL.h>
#include "TSDL_Utility.hpp"
#include "TSDL_SDLmask.hpp"
#include "TSDL_Macro.hpp"
#include "TSDL_Window.hpp"

namespace TSDL
{
    class _TSDL_GET_MASK_TYPE(Texture);
    using _SDL_Texture = _TSDL_GET_MASK_TYPE(Texture);

    class TSDL_Renderer
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
        TSDL_Renderer(TSDL_Window& window);
        TSDL_Renderer(TSDL_Window& window, Uint32 flags);

        ~TSDL_Renderer();

        operator SDL_Renderer*() const;

        std::optional<_SDL_Texture> target();
        int target(std::optional<std::reference_wrapper<_SDL_Texture>> texture);

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
            _SDL_Texture texture, 
            std::optional<const std::reference_wrapper<rect>> srcrect, 
            std::optional<const std::reference_wrapper<rect>> dstrect
        );
        /*
        * like copy_from(texture, srcrect, dstrct) but after copy, optionally rotating it by angle around the given 
        * center and also optionally flipping it.
        */
        int copy_from(_SDL_Texture texture, 
                      std::optional<const std::reference_wrapper<rect>> srcrect, 
                      std::optional<const std::reference_wrapper<rect>> dstrect, 
                      const double angle, 
                      std::optional<const std::reference_wrapper<point_2d>> center,
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
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Renderer)
}

#include "TSDL_Texture.hpp"

#endif