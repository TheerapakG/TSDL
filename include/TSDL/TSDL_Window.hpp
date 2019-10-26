#ifndef TSDL_WINDOW_
#define TSDL_WINDOW_

#include <SDL.h>
#include "TSDL_SDLmask.hpp"
#include "TSDL_Surface.hpp"
#include "TSDL_Display.hpp"

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

        SDL_Surface* window_surface(void);

        TSDL_Surface window_surface_object(void);

        int update_window_surface(void);

        TSDL_Display display(void);

        void window_position(int x, int y);
        void window_position(TSDL_Display& display, int x, int y);
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

void _tsdl_window_py(const py::module& m);

#endif

#endif