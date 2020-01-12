#include "TSDL/TSDL_Music.hpp"
#include "TSDL/TSDL_Utility.hpp"

TSDL::TSDL_Music::TSDL_Music(Mix_Music* ptr): TSDL_Music(ptr, false) {}

TSDL::TSDL_Music::TSDL_Music(Mix_Music* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Music::TSDL_Music(const std::string& file): _destroy(true)
{
    Mix_Music* _t_internal_ptr = Mix_LoadMUS(file.c_str());
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Music could not be loaded! SDL_Mixer_Error: " + std::string(Mix_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Music::~TSDL_Music()
{
    if(_destroy && (_internal_ptr != nullptr)) Mix_FreeMusic(_internal_ptr);
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

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Music)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Music)

void _tsdl_music_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Music)>(m, "Music")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Music)<const std::string>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Music, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Music, exit_ctx));
    py::class_<TSDL::TSDL_Music>(m, "_Music");
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Music)>(m, "_SDL_Music");
    py::implicitly_convertible<TSDL::TSDL_Music, TSDL::_TSDL_GET_MASK_TYPE(Music)>();
}

#endif