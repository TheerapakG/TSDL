#include "TSDL/TSDL_Main.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>

TSDL::TSDL::TSDL()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        SDL_Quit();
        throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }
    else
    {
        int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            IMG_Quit();
            SDL_Quit();
            throw std::runtime_error("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
        }
        else
        {
            return;
        }
    }
}

TSDL::TSDL::~TSDL()
{
    IMG_Quit();
    SDL_Quit();
}

#ifdef TSDL_EXPOSE_PYBIND11

#define TSDL_TSDL TSDL

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, TSDL)

#undef TSDL_TSDL

#endif