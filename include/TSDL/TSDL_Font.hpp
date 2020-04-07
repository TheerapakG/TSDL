#ifndef TSDL_FONT_
#define TSDL_FONT_

#include <SDL2/SDL_ttf.h>
#include "TSDL_Macro.hpp"
#include "TSDL_Buffer.hpp"
#include <vector>

#ifdef TSDL_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#ifdef TSDL_EXPOSE_PYBIND11
#include "TSDL_PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Font)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Font)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Font)
    _PY_GET_CONTEXTMANAGER(Font)<const std::string, int>, 
    _PY_GET_CONTEXTMANAGER(Font)<const std::string, int, long>, 
    _PY_GET_CONTEXTMANAGER(Font)<TSDL::TSDL_Buffer, int>,
    _PY_GET_CONTEXTMANAGER(Font)<TSDL::TSDL_Buffer, int, long>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class TSDL_Font
    {
        private:
        TTF_Font* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = TTF_Font;

        _PY_DECLARE_TYPEERASE_OWNER(Font)

        TSDL_DECLARE_CONSTRUCT(Font)

        TSDL_Font(TTF_Font* ptr);
        TSDL_Font(TTF_Font* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Font(const std::_TSDL_U8(string)& file, int pt);
        TSDL_Font(const std::_TSDL_U8(string)& file, int pt, long index);
        TSDL_Font(TSDL_Buffer& buffer, int pt);
        TSDL_Font(TSDL_Buffer& buffer, int pt, long index);
        TSDL_Font(const void* mem, size_t size, int pt);
        TSDL_Font(const void* mem, size_t size, int pt, long index);

        ~TSDL_Font();

        operator TTF_Font*() const;
    };

    #ifdef TSDL_USE_FONTCONFIG
    std::vector<std::_TSDL_U8(string)> get_all_font_filename();
    std::_TSDL_U8(string) get_family_font_filename(const std::_TSDL_U8(string)& family);
    std::_TSDL_U8(string) get_name_font_filename(const std::_TSDL_U8(string)& name);
    #endif
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Font)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Font)
}

void _tsdl_font_py(const py::module& m);

#endif

#endif