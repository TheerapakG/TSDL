//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/TSDL_Main.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <filesystem>

namespace TSDL
{
    SDL_Event null_event;
}

TSDL::TSDL::TSDL(): TSDL::TSDL(44100) {}

TSDL::TSDL::TSDL(int frequency)
{
    std::filesystem::create_directory(std::filesystem::current_path()/"cache");
    
    std::cout << "initializing SDL..." << std::endl;
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        std::string e = SDL_GetError();
        SDL_Quit();
        safe_throw<std::runtime_error>("SDL could not initialize! SDL_Error: " + e);
    }
    else
    {
        Uint32 userEventType = SDL_RegisterEvents(1);
        if (userEventType == ((Uint32)-1))
        {
            SDL_Quit();
            safe_throw<std::runtime_error>("TSDL Error: could not register null event");
        }
        else
        {
            SDL_zero(null_event); /* or SDL_zero(event) */
            null_event.type = userEventType;
            null_event.user.code = 0;
            null_event.user.data1 = 0;
            null_event.user.data2 = 0;

            std::cout << "initializing SDL_image..." << std::endl;
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                std::string e = IMG_GetError();
                IMG_Quit();
                SDL_Quit();
                safe_throw<std::runtime_error>("SDL_image could not initialize! SDL_image Error: " + e);
            }
            else
            {
                std::cout << "initializing SDL_mixer..." << std::endl;
                if( Mix_OpenAudio( frequency, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
                {
                    std::string e = Mix_GetError();
                    Mix_Quit();
                    IMG_Quit();
                    SDL_Quit();
                    safe_throw<std::runtime_error>("SDL_mixer could not initialize! SDL_mixer Error: " + e);
                }
                else
                {
                    std::cout << "initializing SDL_ttf..." << std::endl;
                    if(TTF_Init() != 0)
                    {
                        std::string e = TTF_GetError();
                        TTF_Quit();
                        Mix_Quit();
                        IMG_Quit();
                        SDL_Quit();
                        safe_throw<std::runtime_error>("SDL_ttf could not initialize! SDL_ttf Error: " + e);
                    }
                    else
                    {
                        #ifdef TSDL_USE_FONTCONFIG
                        std::cout << "initializing Fontconfig..." << std::endl;
                        if(!FcInitLoadConfig())
                        {
                            FcFini();
                            TTF_Quit();
                            Mix_Quit();
                            IMG_Quit();
                            SDL_Quit();
                            safe_throw<std::runtime_error>("Fontconfig could not initialize!");
                        }

                        auto local_fontconf_p = std::filesystem::current_path()/"fonts.conf";
                        if(exists(local_fontconf_p))
                        {
                            std::cout << "Setting Fontconfig..." << std::endl;
                            FcConfig* config = FcConfigCreate();
                            if(!FcConfigParseAndLoad(config, (const FcChar8*)((local_fontconf_p).string().c_str()), true)) //eww
                            {
                                FcFini();
                                TTF_Quit();
                                Mix_Quit();
                                IMG_Quit();
                                SDL_Quit();
                                safe_throw<std::runtime_error>("Could not parse fontconfig file!");
                            }
                            FcConfig* last_config = FcConfigGetCurrent();
                            if(!FcConfigSetCurrent(config))
                            {
                                FcFini();
                                TTF_Quit();
                                Mix_Quit();
                                IMG_Quit();
                                SDL_Quit();
                                safe_throw<std::runtime_error>("Could not set font config!");
                            }
                            FcConfigDestroy(last_config);
                        }
                        #endif

                        #ifndef __cpp_exceptions
                        constructed = true;
                        #endif
                        return;
                    }
                }
            }
        }
    }
}

TSDL::TSDL::~TSDL()
{
    #ifdef TSDL_USE_FONTCONFIG
    FcConfigDestroy(FcConfigGetCurrent());
    FcFini();
    #endif
    TTF_Quit();
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
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(TSDL, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(TSDL, exit_ctx));
    py::class_<TSDL::TSDL>(m, "_TSDL")
        .def(py::init<>());
}

#undef TSDL_TSDL

#endif