#include "TSDL/TSDL_Main.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdexcept>

TSDL::TSDL::TSDL(): TSDL::TSDL(44100) {}

TSDL::TSDL::TSDL(int frequency)
{
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        std::string e = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error("SDL could not initialize! SDL_Error: " + e);
    }
    else
    {
        int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            std::string e = IMG_GetError();
            IMG_Quit();
            SDL_Quit();
            throw std::runtime_error("SDL_image could not initialize! SDL_image Error: " + e);
        }
        else
        {
            if( Mix_OpenAudio( frequency, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
            {
                std::string e = Mix_GetError();
                Mix_Quit();
                IMG_Quit();
                SDL_Quit();
                throw std::runtime_error("SDL_mixer could not initialize! SDL_mixer Error: " + e);
            }
            else
            {
                return;
            }
        }
    }
}

TSDL::TSDL::~TSDL()
{
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

#ifdef TSDL_EXPOSE_PYBIND11

#define TSDL_TSDL TSDL

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, TSDL)

void _tsdl_main_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(TSDL)>(m, "TSDL")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(TSDL)<>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(TSDL, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(TSDL, exit_ctx));
    py::class_<TSDL::TSDL>(m, "_TSDL")
        .def(py::init<>());
}

#undef TSDL_TSDL

#endif