#include "TSDL/TSDL_Music.hpp"
#include <stdexcept>

TSDL::TSDL_Music::TSDL_Music(Mix_Music* ptr): TSDL_Music(ptr, false) {}

TSDL::TSDL_Music::TSDL_Music(Mix_Music* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Music::TSDL_Music(const std::string& file): _destroy(true)
{
    Mix_Music* _t_internal_ptr = Mix_LoadMUS(file.c_str());
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Music could not be loaded! SDL_Mixer_Error: " + std::string(Mix_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Music::~TSDL_Music()
{
    if(_destroy) Mix_FreeMusic(_internal_ptr);
}

TSDL::TSDL_Music::operator Mix_Music*() const
{
    return _internal_ptr;
}

int TSDL::play(const TSDL::_SDL_Music& music, int loops)
{
    int _t = Mix_PlayMusic(music, loops);
    if(_t != 0)
    {
        throw std::runtime_error("Cannot play music! SDL_Mixer_Error: " + std::string(Mix_GetError()));
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

_TSDL_EXPAND_DECLARE_MASK_MIX(TSDL, Music)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Music)

#endif