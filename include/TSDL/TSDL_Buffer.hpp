#ifndef TSDL_BUFFER_
#define TSDL_BUFFER_

#include <SDL.h>
#include "TSDL_SDLmask.hpp"
#include "TSDL_Macro.hpp"
#include <string>

#ifdef TSDL_EXPOSE_PYBIND11
#include "TSDL_PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Buffer)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Buffer)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Buffer)
    _PY_GET_CONTEXTMANAGER(Buffer)<const std::string, const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class TSDL_Buffer
    {
        private:
        SDL_RWops* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = SDL_RWops;

        _PY_DECLARE_TYPEERASE_OWNER(Buffer)

        TSDL_DECLARE_CONSTRUCT(Buffer)

        TSDL_Buffer(SDL_RWops* ptr);
        TSDL_Buffer(SDL_RWops* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Buffer(const std::string& file, const std::string& mode);
        TSDL_Buffer(void* mem, size_t size);
        TSDL_Buffer(const void* mem, size_t size);

        ~TSDL_Buffer();

        operator SDL_RWops*() const;
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Buffer)
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Buffer)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Buffer)
}

void _tsdl_buffer_py(const py::module& m);

#endif

#endif