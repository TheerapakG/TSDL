#ifndef TSDL_CHUNK_
#define TSDL_CHUNK_

#include <SDL.h>
#include <SDL_mixer.h>
#include "TSDL_SDLmask.hpp"

namespace TSDL
{
    class TSDL_Chunk
    {
        private:
        Mix_Chunk* _internal_ptr;
        bool _destroy;

        public:
        TSDL_Chunk(Mix_Chunk* ptr);
        TSDL_Chunk(Mix_Chunk* ptr, bool handle_destroy);
        TSDL_Chunk(const std::string& file);
        ~TSDL_Chunk();

        operator Mix_Chunk*() const;
    };

    _TSDL_EXPAND_DEFINE_MASK_MIX(Chunk)
    using _SDL_Chunk = _TSDL_GET_MASK_TYPE(Chunk);

    /*
    These functions control chunk playback
    If limit, provide it in milliseconds
    */
    inline int play(const _SDL_Chunk& chunk, int loops);
    inline int play(const _SDL_Chunk& chunk, int loops, int limit);
    inline int play(int channel, const _SDL_Chunk& chunk, int loops);
    int play(int channel, const TSDL::_SDL_Chunk& chunk, int loops, int limit);
}

#ifdef TSDL_EXPOSE_PYBIND11

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Chunk)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Chunk)
    _PY_GET_CONTEXTMANAGER(Chunk)<const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Chunk)
}

#define _TSDL_CHUNK_PY                                                                                                \
    py::class_<_PY::_PY_GET_TYPEERASE(Chunk)>(m, "Chunk")                                                             \
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Chunk)<const std::string>())                                              \
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Chunk, enter_ctx), py::return_value_policy::reference)     \
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Chunk, exit_ctx));                                          \
    py::class_<TSDL::TSDL_Chunk>(m, "_Chunk");                                                                        \
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Chunk)>(m, "_SDL_Chunk");                                                    \
    py::implicitly_convertible<TSDL::TSDL_Chunk, TSDL::_TSDL_GET_MASK_TYPE(Chunk)>();                                 \

#endif

#endif