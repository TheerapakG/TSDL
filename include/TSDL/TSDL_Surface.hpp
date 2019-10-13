#ifndef TSDL_SURFACE_
#define TSDL_SURFACE_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "TSDL_Utility.hpp"
#include "TSDL_SDLmask.hpp"
#include <string>

namespace TSDL
{
    class _TSDL_GET_MASK_TYPE(Surface);
    using _SDL_Surface = _TSDL_GET_MASK_TYPE(Surface);

    enum class TTF_Rendermethod
    {
        Solid,
        Shaded,
        Blended,
    };

    class TSDL_Surface
    {
        private:
        SDL_Surface* _internal_ptr;
        bool _destroy;

        public:
        TSDL_Surface(SDL_Surface* ptr);

        TSDL_Surface(SDL_Surface* ptr, bool handle_destroy);

        TSDL_Surface(const std::string& file);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b, TTF_Rendermethod m);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b, Uint8 a, TTF_Rendermethod m);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 br, Uint8 bg, Uint8 bb);

        TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 fa, Uint8 br, Uint8 bg, Uint8 bb, Uint8 ba);

        ~TSDL_Surface();

        operator SDL_Surface*() const;

        /*
        the format of the pixels stored in the surface; see SDL_PixelFormat for details (read-only)
        */
        const SDL_PixelFormat* get_format() const;

        /*
        Use this function to perform a fast surface copy to a destination surface.

        Remarks:
        The width and height in srcrect determine the size of the copied rectangle.
        Only the position is used in the dstrect (the width and height are ignored).
        Blits with negative dstrect coordinates will be clipped properly.
        */
        int copy_from(_SDL_Surface src, const rect& srcrect, rect& dstrect);

        /*
        Use this function to perform a scaled surface copy to a destination surface.
        */
        int scale_from(_SDL_Surface src, const rect& srcrect, rect& dstrect);

        int fill_rect(const rect& rect, Uint32 color);

        int fill_rect(const rect& rect, Uint8 r, Uint8 g, Uint8 b);

        int set_color_key(bool flag, Uint32 key);

        int set_color_key(bool flag, Uint8 r, Uint8 g, Uint8 b);

        Uint32 map_rgb(Uint8 r, Uint8 g, Uint8 b) const;

        TSDL_Surface get_converted_surface(const SDL_PixelFormat* fmt, Uint32 flags) const;

        void convert_surface(const SDL_PixelFormat* fmt, Uint32 flags);
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Surface)
}

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Surface)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Surface)
    _PY_GET_CONTEXTMANAGER(Surface)<const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Surface)
}

#define _TSDL_SURFACE_PY                                                                                              \
    py::class_<_PY::_PY_GET_TYPEERASE(Surface)>(m, "Surface")                                                         \
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Surface)<const std::string>())                                            \
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Surface, enter_ctx), py::return_value_policy::reference)   \
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Surface, exit_ctx));                                        \
    py::class_<TSDL::TSDL_Surface>(m, "_Surface")                                                                     \
        .def("get_format", &TSDL::TSDL_Surface::get_format)                                                           \
        .def("copy_from", &TSDL::TSDL_Surface::copy_from)                                                             \
        .def("scale_from", &TSDL::TSDL_Surface::scale_from)                                                           \
        .def("fill_rect", py::overload_cast<const rect&, Uint32>(&TSDL::TSDL_Surface::fill_rect))                     \
        .def("fill_rect", py::overload_cast<const rect&, Uint8, Uint8, Uint8>(&TSDL::TSDL_Surface::fill_rect))        \
        .def("set_color_key", py::overload_cast<bool, Uint32>(&TSDL::TSDL_Surface::set_color_key))                    \
        .def("set_color_key", py::overload_cast<bool, Uint8, Uint8, Uint8>(&TSDL::TSDL_Surface::set_color_key))       \
        .def("map_rgb", &TSDL::TSDL_Surface::map_rgb)                                                                 \
        .def("get_converted_surface", &TSDL::TSDL_Surface::get_converted_surface)                                     \
        .def("convert_surface", &TSDL::TSDL_Surface::convert_surface);                                                \
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Surface)>(m, "_SDL_Surface");                                                \
    py::implicitly_convertible<TSDL::TSDL_Surface, TSDL::_TSDL_GET_MASK_TYPE(Surface)>();                             \

#endif

#endif