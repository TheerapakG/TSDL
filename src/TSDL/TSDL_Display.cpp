#include "TSDL/TSDL_Display.hpp"
#include <stdexcept>

TSDL::TSDL_Display::TSDL_Display(int index): _index(index) {};

TSDL::rect TSDL::TSDL_Display::get_bound()
{
    SDL_Rect _rect;
    int _t = SDL_GetDisplayBounds(_index, &_rect);
    if(_t != 0)
    {
        throw std::runtime_error("Could not get display bound! SDL Error: " + std::string(SDL_GetError()));
    }
    return _rect;
}