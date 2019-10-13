#include "TSDL/TSDL_Utility.hpp"

TSDL::point_2d::point_2d(int x, int y): SDL_Point{x, y} {}

TSDL::point_2d::point_2d(SDL_Point p): SDL_Point(p) {}

TSDL::point_2d::point_2d(_point_2d p): SDL_Point({std::get<0>(p), std::get<1>(p)}) {}

TSDL::point_2d::operator _point_2d()
{
    return std::make_tuple(x, y);
}

TSDL::rect::rect(int x, int y, int w, int h): SDL_Rect{x, y, w, h} {}

TSDL::rect::rect(SDL_Rect r): SDL_Rect(r) {}

TSDL::rect::rect(_rect r): SDL_Rect(
    {
        std::get<0>(r).x, std::get<0>(r).y,
        std::get<1>(r).x, std::get<1>(r).y
    }
) {}

TSDL::rect::operator _rect()
{
    return std::make_tuple(point_2d(x, y), point_2d(w, h));
}