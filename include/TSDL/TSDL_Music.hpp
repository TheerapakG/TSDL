#ifndef TSDL_MUSIC_
#define TSDL_MUSIC_

#include <SDL.h>
#include <SDL_mixer.h>
#include "TSDL_SDLmask.hpp"

namespace TSDL
{
    class TSDL_Music
    {
        private:
        Mix_Music* _internal_ptr;
        bool _destroy;

        public:
        TSDL_Music(Mix_Music* ptr);
        TSDL_Music(Mix_Music* ptr, bool handle_destroy);
        TSDL_Music(const std::string& file);
        ~TSDL_Music();

        operator Mix_Music*() const;
    };

    _TSDL_EXPAND_DEFINE_MASK_MIX(Music)
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

#define _TSDL_MUSIC_PY                                                                                               \
    py::class_<_PY::_PY_GET_TYPEERASE(Music)>(m, "Music")                                                             \
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Music)<const std::string>())                                              \
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Music, enter_ctx), py::return_value_policy::reference)     \
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Music, exit_ctx));                                          \
    py::class_<TSDL::TSDL_Music>(m, "_Music");                                                                       \
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Music)>(m, "_SDL_Music");                                                    \
    py::implicitly_convertible<TSDL::TSDL_Music, TSDL::_TSDL_GET_MASK_TYPE(Music)>();                                 \

#endif

#endif