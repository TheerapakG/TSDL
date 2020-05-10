//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_MUSIC_
#define TSDL_MUSIC_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "TSDL_Macro.hpp"
#include "TSDL_Compat.hpp"

#ifdef TSDL_EXPOSE_PYBIND11
#include "TSDL_PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(Music)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(Music)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Music)
    _PY_GET_CONTEXTMANAGER(Music)<const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    class TSDL_Music
    {
        private:
        Mix_Music* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = Mix_Music;

        _PY_DECLARE_TYPEERASE_OWNER(Music)

        TSDL_DECLARE_CONSTRUCT(Music)

        TSDL_Music(Mix_Music* ptr);
        TSDL_Music(Mix_Music* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Music(const std::_TSDL_U8(string)& file);

        ~TSDL_Music();

        operator Mix_Music*() const;
    };

    /*
    These functions control music playback
    */
    int play(const TSDL_Music& music, int loops);
    void pause();
    void resume();
    void hault();

    /*
    These function check state of music playback
    */
    bool is_playing();
    bool is_paused();
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Music)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Music)
}

void _tsdl_music_py(const py::module& m);

#endif

#endif