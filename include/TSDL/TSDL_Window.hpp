#ifndef TSDL_WINDOW_
#define TSDL_WINDOW_

#include <SDL2/SDL.h>
#include "TSDL_Surface.hpp"
#include "TSDL_Display.hpp"
#include "TSDL_Macro.hpp"

#ifdef TSDL_EXPOSE_PYBIND11
#include "TSDL_PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Window)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Window)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Window)
    _PY_GET_CONTEXTMANAGER(Window)<const std::_TSDL_U8(string), int, int, int, int, Uint32>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class TSDL_Window
    {
        private:
        SDL_Window* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = SDL_Window;

        _PY_DECLARE_TYPEERASE_OWNER(Window)

        TSDL_DECLARE_CONSTRUCT(Window)

        TSDL_Window(SDL_Window* ptr);
        TSDL_Window(SDL_Window* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Window(const std::_TSDL_U8(string)& title, int x, int y, int w, int h, Uint32 flags);

        ~TSDL_Window();

        operator SDL_Window*() const;

        SDL_Surface* window_surface(void);

        TSDL_Surface window_surface_object(void);

        int update_window_surface(void);

        TSDL_Display display(void);

        void window_position(int x, int y);
        void window_position(const TSDL_Display& display, int x, int y);
    };
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Window)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Window)
}

void _tsdl_window_py(const py::module& m);

#endif

#endif