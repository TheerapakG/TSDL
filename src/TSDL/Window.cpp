//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Window.hpp"
#include "TSDL/Utility.hpp"
#include <string>
#include <iostream>

TSDL_DEFINE_CONSTRUCT(TSDL, Window)

TSDL::Window::Window(SDL_Window* ptr): Window(ptr, false) {}

TSDL::Window::Window(SDL_Window* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::Window::Window(const std::_TSDL_U8(string)& title, int x, int y, int w, int h, Uint32 flags)
{
    std::cout << "Creating window named " << title.c_str() << std::endl;
    SDL_Window* _t_internal_ptr = SDL_CreateWindow(reinterpret_cast<const char*>(title.c_str()), x, y, w, h, flags);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

TSDL::Window::~Window()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_DestroyWindow(_internal_ptr);
}

TSDL::Window::operator SDL_Window*() const
{
    return _internal_ptr;
}

SDL_Surface* TSDL::Window::window_surface(void)
{
    SDL_Surface* _t = SDL_GetWindowSurface(_internal_ptr);
    if(_t == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot get surface from window! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::Surface TSDL::Window::window_surface_object(void)
{
    return TSDL::Surface(this->window_surface());
}

int TSDL::Window::update_window_surface(void)
{
    int _t = SDL_UpdateWindowSurface(_internal_ptr);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot update window surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::Display TSDL::Window::display(void)
{
    int _t = SDL_GetWindowDisplayIndex(_internal_ptr);
    if(_t < 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot get display index! SDL_Error: " + std::string(SDL_GetError()));
        // TODO: noexcept signify error
    }
    return Display(_t);
}

Uint32 TSDL::Window::id(void)
{
    Uint32 _t = SDL_GetWindowID(_internal_ptr);
    if (_t == 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot get window id! SDL_Error: " + std::string(SDL_GetError()));
        // TODO: noexcept signify error
    }
    return _t;
}

void TSDL::Window::window_position(int x, int y)
{
    this->window_position(this->display(), x, y);
}

void TSDL::Window::window_position(const Display& display, int x, int y)
{
    int _index = display.index();
    TSDL::rect _bound = display.bound();

    if(SDL_WINDOWPOS_ISCENTERED(x)) x = SDL_WINDOWPOS_CENTERED_DISPLAY(_index);
    else if(SDL_WINDOWPOS_ISUNDEFINED(x)) x = SDL_WINDOWPOS_UNDEFINED_DISPLAY(_index);
    else x += _bound.x;

    if(SDL_WINDOWPOS_ISCENTERED(y)) y = SDL_WINDOWPOS_CENTERED_DISPLAY(display.index());
    else if(SDL_WINDOWPOS_ISUNDEFINED(y)) y = SDL_WINDOWPOS_UNDEFINED_DISPLAY(display.index());
    else y += _bound.y;

    SDL_SetWindowPosition(_internal_ptr, x, y);
}

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Window)

void _tsdl_window_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Window)>(m, "Window")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Window)<const std::_TSDL_U8(string), int, int, int, int, Uint32>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, exit_ctx));
    py::class_<TSDL::Window>(m, "_Window")
        .def("window_surface_object", &TSDL::Window::window_surface_object)
        .def("update_window_surface", &TSDL::Window::update_window_surface);
}

#endif