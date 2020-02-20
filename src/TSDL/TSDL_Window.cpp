#include "TSDL/TSDL_Window.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include <string>
#include <iostream>

TSDL_DEFINE_CONSTRUCT(TSDL, Window)

TSDL::TSDL_Window::TSDL_Window(SDL_Window* ptr): TSDL_Window(ptr, false) {}

TSDL::TSDL_Window::TSDL_Window(SDL_Window* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Window::TSDL_Window(const std::string& title, int x, int y, int w, int h, Uint32 flags)
{
    std::cout << "Creating window named " << title.c_str() << std::endl;
    SDL_Window* _t_internal_ptr = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

TSDL::TSDL_Window::~TSDL_Window()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_DestroyWindow(_internal_ptr);
}

TSDL::TSDL_Window::operator SDL_Window*() const
{
    return _internal_ptr;
}

SDL_Surface* TSDL::TSDL_Window::window_surface(void)
{
    SDL_Surface* _t = SDL_GetWindowSurface(_internal_ptr);
    if(_t == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot get surface from window! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::TSDL_Surface TSDL::TSDL_Window::window_surface_object(void)
{
    return TSDL::TSDL_Surface(this->window_surface());
}

int TSDL::TSDL_Window::update_window_surface(void)
{
    int _t = SDL_UpdateWindowSurface(_internal_ptr);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot update window surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::TSDL_Display TSDL::TSDL_Window::display(void)
{
    int _t = SDL_GetWindowDisplayIndex(_internal_ptr);
    if(_t < 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot get display index! SDL_Error: " + std::string(SDL_GetError()));
        // TODO: noexcept signify error
    }
    return TSDL_Display(_t);
}

void TSDL::TSDL_Window::window_position(int x, int y)
{
    this->window_position(this->display(), x, y);
}

void TSDL::TSDL_Window::window_position(const TSDL_Display& display, int x, int y)
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
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Window)<const std::string, int, int, int, int, Uint32>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, exit_ctx));
    py::class_<TSDL::TSDL_Window>(m, "_Window")
        .def("window_surface_object", &TSDL::TSDL_Window::window_surface_object)
        .def("update_window_surface", &TSDL::TSDL_Window::update_window_surface);
}

#endif