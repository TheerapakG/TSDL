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
#include <functional>
#include <string>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <stack>

namespace TSDL
{
    SDL_Event null_event;
    #ifdef TSDL_USE_PANGOCAIRO
    PangoContext* default_pango_context = nullptr;
    #endif
}

TSDL::TSDL::TSDL(): TSDL::TSDL(44100) {}

namespace
{
    class RAIIinit
    {
        private:
        std::function<void(void)> _uninit;
        bool _success = true;

        public:
        template <typename T, typename U, typename _Err, typename _Throw_Err>
        RAIIinit(
            std::function<T(void)> init, 
            std::function<bool(U)> success_test, 
            std::function<void(void)> uninit, 
            std::function<_Err(void)> get_error, 
            std::function<void(const _Throw_Err&)> throw_error
        ): _uninit(uninit)
        {
            if(!success_test(init()))
            {
                const _Throw_Err _err = get_error();
                _uninit();
                _success = false;
                throw_error(_err);
            }
        }

        template <typename T, typename U>
        RAIIinit(
            std::function<T(void)> init, 
            std::function<bool(U)> success_test, 
            std::function<void(void)> uninit,
            std::function<void(void)> throw_error
        ): _uninit(uninit)
        {
            if(!success_test(init()))
            {
                _uninit();
                _success = false;
                throw_error();
            }
        }

        ~RAIIinit()
        {
            if(_success) _uninit();
        }

        bool init_success()
        {
            return _success;
        }
    };

    std::stack <RAIIinit> _all_init;

    void _uninit_all()
    {
        while(!_all_init.empty()) _all_init.pop();
    }
}

TSDL::TSDL::TSDL(int frequency)
{
    using namespace std::placeholders;

    std::filesystem::create_directory(std::filesystem::current_path()/"cache");
    
    _all_init.emplace(
        std::function(
            []()
            {
                std::cout << "initializing SDL..." << std::endl;
                return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
            }
        ),
        std::function<bool(int)>(std::bind(std::equal_to<int>(), _1, 0)),
        std::function(SDL_Quit),
        std::function(SDL_GetError),
        std::function(
            [](const std::string& sdl_err)
            {
                safe_throw<std::runtime_error>("SDL could not initialize! SDL_Error: " + sdl_err);
            }
        )
    );

    if(!_all_init.top().init_success())
    {
        _uninit_all();
        return;
    }
    
    Uint32 userEventType = SDL_RegisterEvents(1);
    if (userEventType == ((Uint32)-1))
    {
        _uninit_all();
        safe_throw<std::runtime_error>("TSDL Error: could not register null event");
        return;
    }
    else
    {
        SDL_zero(null_event); /* or SDL_zero(event) */
        null_event.type = userEventType;
        null_event.user.code = 0;
        null_event.user.data1 = 0;
        null_event.user.data2 = 0;
    }

    #ifdef TSDL_USE_SDLIMG
    _all_init.emplace(
        std::function(
            []() -> int
            {
                std::cout << "initializing SDL_image..." << std::endl;
                int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
                return IMG_Init(imgFlags) & imgFlags;
            }
        ),
        std::function(std::logical_not<int>()),
        std::function(IMG_Quit),
        std::function(IMG_GetError),
        std::function(
            [](const std::string& img_err)
            {
                safe_throw<std::runtime_error>("SDL_image could not initialize! SDL_image Error: " + img_err);
            }
        )
    );

    if(!_all_init.top().init_success())
    {
        _uninit_all();
        return;
    }
    #endif

    _all_init.emplace(
        std::function(
            [frequency]()
            {
                std::cout << "initializing SDL_mixer..." << std::endl;
                return Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, 2048);
            }
        ),
        std::function<bool(int)>(std::bind(std::equal_to<int>(), _1, 0)),
        std::function(Mix_Quit),
        std::function(Mix_GetError),
        std::function(
            [](const std::string& mix_err)
            {
                safe_throw<std::runtime_error>("SDL_mixer could not initialize! SDL_mixer Error: " + mix_err);
            }
        )
    );

    if(!_all_init.top().init_success())
    {
        _uninit_all();
        return;
    }
            
    #ifdef TSDL_USE_SDLTTF
    _all_init.emplace(
        std::function(
            []()
            {
                std::cout << "initializing SDL_ttf..." << std::endl;
                return TTF_Init();
            }
        ),
        std::function<bool(int)>(std::bind(std::equal_to<int>(), _1, 0)),
        std::function(TTF_Quit),
        std::function(TTF_GetError),
        std::function(
            [](const std::string& ttf_err)
            {
                safe_throw<std::runtime_error>("SDL_ttf could not initialize! SDL_ttf Error: " + ttf_err);
            }
        )
    );

    if(!_all_init.top()->init_success())
    {
        _uninit_all();
        return;
    }
    #endif
                
    #ifdef TSDL_USE_FONTCONFIG
    _all_init.emplace(
        std::function(
            []()
            {
                std::cout << "initializing Fontconfig..." << std::endl;
                return FcInitLoadConfig();
            }
        ),
        std::function(std::logical_not<FcConfig*>()),
        std::function(
            []()
            {
                FcConfigDestroy(FcConfigGetCurrent());
                FcFini();
            }
        ),
        std::function<void(void)>(std::bind(safe_throw<std::runtime_error, std::string>, "Fontconfig could not initialize!"))
    );

    if(!_all_init.top().init_success())
    {
        _uninit_all();
        return;
    }
                        
    auto local_fontconf_p = std::filesystem::current_path()/"fonts.conf";
    if(exists(local_fontconf_p))
    {
        std::cout << "Setting Fontconfig..." << std::endl;
        FcConfig* config = FcConfigCreate();
        if(!FcConfigParseAndLoad(config, (const FcChar8*)((local_fontconf_p).string().c_str()), true)) //eww
        {
            _uninit_all();
            safe_throw<std::runtime_error>("Could not parse fontconfig file!");
            return;
        }
        FcConfig* last_config = FcConfigGetCurrent();
        if(!FcConfigSetCurrent(config))
        {
            FcConfigDestroy(config);
            _uninit_all();
            safe_throw<std::runtime_error>("Could not set font config!");
            return;
        }
        FcConfigDestroy(last_config);
    }
    #endif

    #ifdef TSDL_USE_PANGOCAIRO
    default_pango_context = pango_font_map_create_context(pango_cairo_font_map_get_default());
    #endif

    #ifndef __cpp_exceptions
    constructed = true;
    #endif
}

TSDL::TSDL::~TSDL()
{
    #ifdef TSDL_USE_PANGOCAIRO
    if (default_pango_context != nullptr)
    {
        g_object_unref(default_pango_context);
    }
    #endif
    _uninit_all();
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