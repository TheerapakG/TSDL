//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_CHUNK_
#define TSDL_CHUNK_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Macro.hpp"
#include "Buffer.hpp"

#ifdef TSDL_EXPOSE_PYBIND11
#include "PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Chunk)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Chunk)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Chunk)
    _PY_GET_CONTEXTMANAGER(Chunk)<const std::string>, 
    _PY_GET_CONTEXTMANAGER(Chunk)<TSDL::Buffer>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class Chunk
    {
        private:
        Mix_Chunk* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = Mix_Chunk;

        _PY_DECLARE_TYPEERASE_OWNER(Chunk)

        TSDL_DECLARE_CONSTRUCT(Chunk)

        Chunk(Mix_Chunk* ptr);
        Chunk(Mix_Chunk* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        Chunk(const std::_TSDL_U8(string)& file);
        Chunk(Buffer& buffer);
        Chunk(const void* mem, size_t size);

        ~Chunk();

        operator Mix_Chunk*() const;
    };

    /*
    These functions control chunk playback
    If limit, provide it in milliseconds
    */
    int play(const Chunk& chunk, int loops);
    int play(const Chunk& chunk, int loops, int limit);
    int play(int channel, const Chunk& chunk, int loops);
    int play(int channel, const Chunk& chunk, int loops, int limit);
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Chunk)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Chunk)
}

void _tsdl_chunk_py(const py::module& m);

#endif

#endif