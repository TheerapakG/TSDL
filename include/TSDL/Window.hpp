//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_WINDOW_
#define TSDL_WINDOW_

#include <SDL2/SDL.h>
#include "Surface.hpp"
#include "Display.hpp"
#include "Macro.hpp"

#ifdef TSDL_EXPOSE_PYBIND11
#include "PY_TypeErase.hpp"
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
    class Window
    {
        private:
        SDL_Window* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = SDL_Window;

        _PY_DECLARE_TYPEERASE_OWNER(Window)

        TSDL_DECLARE_CONSTRUCT(Window)

        Window(SDL_Window* ptr);
        Window(SDL_Window* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Window(const std::_TSDL_U8(string)& title, int x, int y, int w, int h, Uint32 flags);

        ~Window();

        operator SDL_Window*() const;

        SDL_Surface* window_surface(void);

        Surface window_surface_object(void);

        int update_window_surface(void);

        Display display(void);

        Uint32 id(void);

        void window_position(int x, int y);
        void window_position(const Display& display, int x, int y);
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