//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Music.hpp"
#include "TSDL/Utility.hpp"

TSDL_DEFINE_CONSTRUCT(TSDL, Music)

TSDL::Music::Music(Mix_Music* ptr): Music(ptr, false) {}

TSDL::Music::Music(Mix_Music* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::Music::Music(const std::_TSDL_U8(string)& file): _destroy(true)
{
    Mix_Music* _t_internal_ptr = Mix_LoadMUS(reinterpret_cast<const char*>(file.c_str()));
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Music could not be loaded! SDL_Mixer_Error: " + std::string(Mix_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::Music::~Music()
{
    if(_destroy && (_internal_ptr != nullptr)) Mix_FreeMusic(_internal_ptr);
}

TSDL::Music::operator Mix_Music*() const
{
    return _internal_ptr;
}

int TSDL::play(const TSDL::Music& music, int loops)
{
    int _t = Mix_PlayMusic(music, loops);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot play music! SDL_Mixer_Error: " + std::string(Mix_GetError()));
    }
    return _t;
}

void TSDL::pause()
{
    Mix_PauseMusic();
}

void TSDL::resume()
{
    Mix_ResumeMusic();
}

void TSDL::hault()
{
    Mix_HaltMusic();
}

bool TSDL::is_playing()
{
    return Mix_PlayingMusic();
}

bool TSDL::is_paused()
{
    return Mix_PausedMusic();
}

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Music)

void _tsdl_music_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Music)>(m, "Music")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Music)<const std::_TSDL_U8(string)>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Music, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Music, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Music, exit_ctx));
    py::class_<TSDL::Music>(m, "_Music");
}

#endif