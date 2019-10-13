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
        point_2d(SDL_Point p);
        point_2d(_point_2d p);

        operator _point_2d();
    };

    using _rect = tuple<point_2d, point_2d>;
    class rect: public SDL_Rect
    {
        public:
        rect(int x, int y, int w, int h);
        rect(SDL_Rect r);
        rect(_rect r);

        operator _rect();
    };
}

#endif