#ifndef TSDL_CHUNK_
#define TSDL_CHUNK_

#include <SDL.h>
#include <SDL_mixer.h>
#include "TSDL_SDLmask.hpp"
#include "TSDL_Buffer.hpp"
#include <string>

namespace TSDL
{
    class TSDL_Chunk
    {
        private:
        Mix_Chunk* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = Mix_Chunk;

        TSDL_Chunk(Mix_Chunk* ptr);
        TSDL_Chunk(Mix_Chunk* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Chunk(const std::string& file);
        TSDL_Chunk(TSDL::TSDL_Buffer& buffer);
        TSDL_Chunk(const void* mem, size_t size);

        ~TSDL_Chunk();

        operator Mix_Chunk*() const;
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Chunk)
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
    _PY_GET_CONTEXTMANAGER(Chunk)<TSDL::TSDL_Buffer>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Chunk)
}

void _tsdl_chunk_py(const py::module& m);

#endif

#endif