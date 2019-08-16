#include "TSDL/TSDL_Window.hpp"
#include <stdexcept>
#include <string>
#include <iostream>

TSDL::TSDL_Window::TSDL_Window(SDL_Window* ptr): TSDL_Window(ptr, false) {}

TSDL::TSDL_Window::TSDL_Window(SDL_Window* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Window::TSDL_Window(const std::string& title, int x, int y, int w, int h, Uint32 flags)
{
    std::cout << "Creating window named " << title.c_str() << std::endl;
    SDL_Window* _t_internal_ptr = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

TSDL::TSDL_Window::~TSDL_Window()
{
    if(_destroy) SDL_DestroyWindow(_internal_ptr);
}

TSDL::TSDL_Window::operator SDL_Window*() const
{
    return _internal_ptr;
}

SDL_Surface* TSDL::TSDL_Window::get_window_surface(void)
{
    SDL_Surface* _t = SDL_GetWindowSurface(_internal_ptr);
    if(_t == NULL)
    {
        throw std::runtime_error("Cannot get surface from window! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::TSDL_Surface TSDL::TSDL_Window::get_window_surface_object(void)
{
    return TSDL::TSDL_Surface(this->get_window_surface());
}

int TSDL::TSDL_Window::update_window_surface(void)
{
    int _t = SDL_UpdateWindowSurface(_internal_ptr);
    if(_t != 0)
    {
        throw std::runtime_error("Cannot update window surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Window)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Window)

#endif