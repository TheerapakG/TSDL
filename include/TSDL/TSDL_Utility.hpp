#ifndef TSDL_UTILITY_
#define TSDL_UTILITY_

#include <SDL.h>
#include <tuple>

namespace TSDL
{
    template <typename ...Ts>
    using tuple = std::tuple<Ts...>;

    using _point_2d = tuple<int, int>;
    class point_2d: public SDL_Point
    {
        public:
        point_2d(int x, int y);
        point_2d(SDL_Point& p);
        point_2d(_point_2d& p);

        operator _point_2d();
    };

    using _rect = tuple<point_2d, point_2d>;
    class rect: public SDL_Rect
    {
        public:
        rect(int x, int y, int w, int h);
        rect(SDL_Rect& r);
        rect(_rect& r);

        operator _rect();
    };

    using _color_rgb = tuple<Uint8, Uint8, Uint8>;
    class color_rgb: public SDL_Color
    {
        public:
        color_rgb(Uint8 r, Uint8 g, Uint8 b);
        color_rgb(SDL_Color& c);
        color_rgb(_color_rgb& c);

        operator _color_rgb();
    };

    using _color_rgba = tuple<Uint8, Uint8, Uint8, Uint8>;
    class color_rgba: public SDL_Color
    {
        public:
        color_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        color_rgba(SDL_Color& c);
        color_rgba(_color_rgba& c);

        color_rgba(color_rgb& c);

        operator _color_rgba();
    };
}

#endif