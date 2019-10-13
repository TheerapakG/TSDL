#ifndef TSDL_PY_CONSTANTS_
#define TSDL_PY_CONSTANTS_

#include <SDL.h>

#define _TSDL_PY_EXPAND_OR_ENUM(T) [](T _self, T _other){ return static_cast<T>(_self|_other); }

#define _TSDL_PY_CONSTANTS_PY                                                                               \
    m.attr("SDL_WINDOWPOS_CENTERED") = SDL_WINDOWPOS_CENTERED;                                              \
    m.attr("SDL_WINDOWPOS_UNDEFINED") = SDL_WINDOWPOS_UNDEFINED;                                            \
    py::enum_<SDL_WindowFlags>(m, "SDL_WindowFlags")                                                        \
        .value("SDL_WINDOW_FULLSCREEN", SDL_WindowFlags::SDL_WINDOW_FULLSCREEN)                             \
        .value("SDL_WINDOW_OPENGL", SDL_WindowFlags::SDL_WINDOW_OPENGL)                                     \
        .value("SDL_WINDOW_SHOWN", SDL_WindowFlags::SDL_WINDOW_SHOWN)                                       \
        .value("SDL_WINDOW_HIDDEN", SDL_WindowFlags::SDL_WINDOW_HIDDEN)                                     \
        .value("SDL_WINDOW_BORDERLESS", SDL_WindowFlags::SDL_WINDOW_BORDERLESS)                             \
        .value("SDL_WINDOW_RESIZABLE", SDL_WindowFlags::SDL_WINDOW_RESIZABLE)                               \
        .value("SDL_WINDOW_MINIMIZED", SDL_WindowFlags::SDL_WINDOW_MINIMIZED)                               \
        .value("SDL_WINDOW_MAXIMIZED", SDL_WindowFlags::SDL_WINDOW_MAXIMIZED)                               \
        .value("SDL_WINDOW_INPUT_GRABBED", SDL_WindowFlags::SDL_WINDOW_INPUT_GRABBED)                       \
        .value("SDL_WINDOW_INPUT_FOCUS", SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS)                           \
        .value("SDL_WINDOW_MOUSE_FOCUS", SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS)                           \
        .value("SDL_WINDOW_FULLSCREEN_DESKTOP", SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP)             \
        .value("SDL_WINDOW_FOREIGN", SDL_WindowFlags::SDL_WINDOW_FOREIGN)                                   \
        .value("SDL_WINDOW_ALLOW_HIGHDPI", SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI)                       \
        .value("SDL_WINDOW_MOUSE_CAPTURE", SDL_WindowFlags::SDL_WINDOW_MOUSE_CAPTURE)                       \
        .value("SDL_WINDOW_ALWAYS_ON_TOP", SDL_WindowFlags::SDL_WINDOW_ALWAYS_ON_TOP)                       \
        .value("SDL_WINDOW_SKIP_TASKBAR", SDL_WindowFlags::SDL_WINDOW_SKIP_TASKBAR)                         \
        .value("SDL_WINDOW_UTILITY", SDL_WindowFlags::SDL_WINDOW_UTILITY)                                   \
        .value("SDL_WINDOW_TOOLTIP", SDL_WindowFlags::SDL_WINDOW_TOOLTIP)                                   \
        .value("SDL_WINDOW_POPUP_MENU", SDL_WindowFlags::SDL_WINDOW_POPUP_MENU)                             \
        .value("SDL_WINDOW_VULKAN", SDL_WindowFlags::SDL_WINDOW_VULKAN)                                     \
        .def("__or__", _TSDL_PY_EXPAND_OR_ENUM(SDL_WindowFlags));                                           \
    py::enum_<SDL_BlendMode>(m, "SDL_BlendMode")                                                            \
        .value("SDL_BLENDMODE_NONE", SDL_BlendMode::SDL_BLENDMODE_NONE)                                     \
        .value("SDL_BLENDMODE_BLEND", SDL_BlendMode::SDL_BLENDMODE_BLEND)                                   \
        .value("SDL_BLENDMODE_ADD", SDL_BlendMode::SDL_BLENDMODE_ADD)                                       \
        .value("SDL_BLENDMODE_MOD", SDL_BlendMode::SDL_BLENDMODE_MOD);                                      \
    py::enum_<SDL_RendererFlags>(m, "SDL_RendererFlags")                                                    \
        .value("SDL_RENDERER_SOFTWARE", SDL_RendererFlags::SDL_RENDERER_SOFTWARE)                           \
        .value("SDL_RENDERER_ACCELERATED", SDL_RendererFlags::SDL_RENDERER_ACCELERATED)                     \
        .value("SDL_RENDERER_PRESENTVSYNC", SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC)                   \
        .value("SDL_RENDERER_TARGETTEXTURE", SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE)                 \
        .def("__or__", _TSDL_PY_EXPAND_OR_ENUM(SDL_RendererFlags));                                         \
    py::enum_<SDL_RendererFlip>(m, "SDL_RendererFlip")                                                      \
        .value("SDL_FLIP_NONE", SDL_RendererFlip::SDL_FLIP_NONE)                                            \
        .value("SDL_FLIP_HORIZONTAL", SDL_RendererFlip::SDL_FLIP_HORIZONTAL)                                \
        .value("SDL_FLIP_VERTICAL", SDL_RendererFlip::SDL_FLIP_VERTICAL)                                    \
        .def("__or__", _TSDL_PY_EXPAND_OR_ENUM(SDL_RendererFlip));                                          \

#endif