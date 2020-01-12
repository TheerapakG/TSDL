#ifndef TSDL_MUSIC_
#define TSDL_MUSIC_

#include <SDL.h>
#include <SDL_mixer.h>
#include "TSDL_SDLmask.hpp"
#include <string>

namespace TSDL
{
    class TSDL_Music
    {
        private:
        Mix_Music* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = Mix_Music;

        TSDL_Music(Mix_Music* ptr);
        TSDL_Music(Mix_Music* ptr, bool handle_destroy);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_Music(const std::string& file);

        ~TSDL_Music();

        operator Mix_Music*() const;
    };

    _TSDL_EXPAND_DEFINE_MASK_TYPE(Music)
    using _SDL_Music = _TSDL_GET_MASK_TYPE(Music);

    /*
    These functions control music playback
    */
    int play(const _SDL_Music& music, int loops);
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

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(Music)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(Music)
    _PY_GET_CONTEXTMANAGER(Music)<const std::string>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(Music)
}

void _tsdl_music_py(const py::module& m);

#endif

#endif