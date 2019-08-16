#ifndef TSDL_WINDOW_
#define TSDL_WINDOW_

#include <SDL.h>
#include "TSDL_SDLmask.hpp"
#include "TSDL_Surface.hpp"

namespace TSDL
{
    class TSDL_Window
    {
        private:
        SDL_Window* _internal_ptr;
        bool _destroy;

        public:
        TSDL_Window(SDL_Window* ptr);
        TSDL_Window(SDL_Window* ptr, bool handle_destroy);
        TSDL_Window(const std::string& title, int x, int y, int w, int h, Uint32 flags);
        ~TSDL_Window();

        operator SDL_Window*() const;

        SDL_Surface* get_window_surface(void);

        TSDL_Surface get_window_surface_object(void);

        int update_window_surface(void);
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Window)
}

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Window)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Window)
    _PY_GET_CONTEXTMANAGER(Window)<const std::string, int, int, int, int, Uint32>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Window)
}

#define _TSDL_WINDOW_PY                                                                                               \
    py::class_<_PY::_PY_GET_TYPEERASE(Window)>(m, "Window")                                                           \
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Window)<const std::string, int, int, int, int, Uint32>())                 \
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, enter_ctx), py::return_value_policy::reference)    \
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Window, exit_ctx));                                         \
    py::class_<TSDL::TSDL_Window>(m, "_Window")                                                                       \
        .def("get_window_surface_object", &TSDL::TSDL_Window::get_window_surface_object)                              \
        .def("update_window_surface", &TSDL::TSDL_Window::update_window_surface);                                     \
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Window)>(m, "_SDL_Window");                                                  \
    py::implicitly_convertible<TSDL::TSDL_Window, TSDL::_TSDL_GET_MASK_TYPE(Window)>();                               \

#endif

#endif