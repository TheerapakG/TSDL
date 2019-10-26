#ifndef TSDL_CHUNK_
#define TSDL_CHUNK_

#include <SDL.h>
#include <SDL_mixer.h>
#include "TSDL_SDLmask.hpp"
#include <string>

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
    int play(int channel, const _SDL_Chunk& chunk, int loops, int limit);
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

void _tsdl_chunk_py(const py::module& m);

#endif

#endif