#include "TSDL/TSDL_Utility.hpp"

#ifndef __cpp_exceptions 
#include <exception>

std::exception* TSDL::_current_exc;
std::function<void(void)> TSDL::unhandleable_exc_handler = std::terminate;

std::exception& TSDL::get_exc()
{
    return *_current_exc;
}

void TSDL::clear_exc()
{
    if(_current_exc != nullptr)
    {
        delete _current_exc;
        _current_exc = nullptr;
    }
}
#endif

TSDL::point_2d::point_2d(int x, int y): SDL_Point{x, y} {}

TSDL::point_2d::point_2d(SDL_Point& p): SDL_Point(p) {}

TSDL::point_2d::point_2d(TSDL::_point_2d& p): SDL_Point({std::get<0>(p), std::get<1>(p)}) {}

TSDL::point_2d TSDL::point_2d::operator+() const
{
    return *this;
}

TSDL::point_2d TSDL::point_2d::operator+(const point_2d& other) const
{
    return TSDL::point_2d(this->x + other.x, this->y + other.y);
}

TSDL::point_2d TSDL::point_2d::operator-() const
{
    return TSDL::point_2d(-(this->x), -(this->y));
}

TSDL::point_2d TSDL::point_2d::operator-(const point_2d& other) const
{
    return TSDL::point_2d(this->x + other.x, this->y + other.y);
}

TSDL::point_2d& TSDL::point_2d::operator+=(const point_2d& other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

TSDL::point_2d& TSDL::point_2d::operator-=(const point_2d& other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

bool operator==(const TSDL::point_2d& lhs, const TSDL::point_2d& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

TSDL::point_2d::operator TSDL::_point_2d()
{
    return std::make_tuple(x, y);
}

TSDL::rect::rect(int x, int y, int w, int h): SDL_Rect{x, y, w, h} {}

TSDL::rect::rect(SDL_Rect& r): SDL_Rect(r) {}

TSDL::rect::rect(TSDL::_rect& r): SDL_Rect(
    {
        std::get<0>(r).x, std::get<0>(r).y,
        std::get<1>(r).x, std::get<1>(r).y
    }
) {}

TSDL::rect::rect(const std::pair<point_2d, point_2d>& point_pair): rect(point_pair.first, point_pair.second) {}

TSDL::rect::rect(const point_2d& topleft, const point_2d& bottomright): SDL_Rect(
    {
        topleft.x, 
        topleft.y, 
        bottomright.x - topleft.x, 
        bottomright.y - topleft.y
    }
) {}

TSDL::rect::operator TSDL::_rect()
{
    return std::make_tuple(TSDL::point_2d(x, y), TSDL::point_2d(w, h));
}

TSDL::color_rgb::color_rgb(Uint8 r, Uint8 g, Uint8 b): SDL_Color{r, g, b, 0xFF} {}

TSDL::color_rgb::color_rgb(SDL_Color& c): SDL_Color(c) {}

TSDL::color_rgb::color_rgb(TSDL::_color_rgb& c): color_rgb(std::get<0>(c), std::get<1>(c), std::get<2>(c)) {}

TSDL::color_rgb::operator TSDL::_color_rgb()
{
    return std::make_tuple(r, g, b);
}

TSDL::color_rgba::color_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a): SDL_Color{r, g, b, a} {}

TSDL::color_rgba::color_rgba(SDL_Color& c): SDL_Color(c) {}

TSDL::color_rgba::color_rgba(TSDL::_color_rgba& c): color_rgba(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c)) {}

TSDL::color_rgba::operator TSDL::_color_rgba()
{
    return std::make_tuple(r, g, b, a);
}