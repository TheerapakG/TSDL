//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_FONT_
#define TSDL_FONT_

#ifdef TSDL_USE_SDLTTF
#include <SDL2/SDL_ttf.h>
#endif

#ifdef TSDL_USE_PANGOCAIRO
#include <pango/pangocairo.h>
#endif

#include "Macro.hpp"
#include "Buffer.hpp"
#include <vector>

#ifdef TSDL_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

// TODO: seperate ttf and pangocairo implementation into their own file

#ifdef TSDL_USE_SDLTTF

#ifdef TSDL_EXPOSE_PYBIND11
#include "PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Font)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Font)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Font)
    _PY_GET_CONTEXTMANAGER(Font)<const std::string, int>, 
    _PY_GET_CONTEXTMANAGER(Font)<const std::string, int, long>, 
    _PY_GET_CONTEXTMANAGER(Font)<TSDL::Buffer, int>,
    _PY_GET_CONTEXTMANAGER(Font)<TSDL::Buffer, int, long>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class Font
    {
        public:
        using SDL_Type = TTF_Font;

        private:
        SDL_Type* _internal_ptr = nullptr;
        bool _destroy;

        public:
        _PY_DECLARE_TYPEERASE_OWNER(Font)

        TSDL_DECLARE_CONSTRUCT(Font)

        Font(SDL_Type* ptr);
        Font(SDL_Type* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Font(const std::_TSDL_U8(string)& file, int pt);
        Font(const std::_TSDL_U8(string)& file, int pt, long index);
        Font(Buffer& buffer, int pt);
        Font(Buffer& buffer, int pt, long index);
        Font(const void* mem, size_t size, int pt);
        Font(const void* mem, size_t size, int pt, long index);

        ~Font();

        operator SDL_Type*() const;

        /*
        Get the recommended spacing between lines of text for this font
        */
        int skip_height() const;
    };
}

#endif // TSDL_USE_SDLTTF

#ifdef TSDL_USE_PANGOCAIRO

#ifdef TSDL_EXPOSE_PYBIND11
#include "PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Font)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Font)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Font)
    _PY_GET_CONTEXTMANAGER(Font)<const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class Font
    {
        public:
        using SDL_Type = PangoFontDescription;

        private:
        SDL_Type* _internal_ptr = nullptr;
        bool _destroy;

        public:
        _PY_DECLARE_TYPEERASE_OWNER(Font)

        TSDL_DECLARE_CONSTRUCT(Font)

        Font(SDL_Type* ptr);
        Font(SDL_Type* ptr, bool handle_destroy);

        Font(const std::_TSDL_U8(string)& font_description);

        ~Font();

        operator SDL_Type*() const;
    };
}
#endif // TSDL_USE_PANGOCAIRO

#ifdef TSDL_USE_FONTCONFIG
namespace TSDL
{
    std::vector<std::_TSDL_U8(string)> get_all_font_filename();
    std::_TSDL_U8(string) get_family_font_filename(const std::_TSDL_U8(string)& family);
    std::_TSDL_U8(string) get_name_font_filename(const std::_TSDL_U8(string)& name);
}
#endif

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Font)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Font)
}

void _tsdl_font_py(const py::module& m);

#endif

#endif