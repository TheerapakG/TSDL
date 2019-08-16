#ifndef TSDL_PY_CONSTANTS_
#define TSDL_PY_CONSTANTS_

#include <SDL.h>

#define _TSDL_PY_CONSTANTS_PY                                                                     \
    m.attr("SDL_WINDOWPOS_UNDEFINED") = SDL_WINDOWPOS_UNDEFINED;                                  \
    py::enum_<SDL_WindowFlags>(m, "SDL_WindowFlags")                                              \
        .value("SDL_WINDOW_FULLSCREEN", SDL_WindowFlags::SDL_WINDOW_FULLSCREEN)                   \
        .value("SDL_WINDOW_OPENGL", SDL_WindowFlags::SDL_WINDOW_OPENGL)                           \
        .value("SDL_WINDOW_SHOWN", SDL_WindowFlags::SDL_WINDOW_SHOWN)                             \
        .value("SDL_WINDOW_HIDDEN", SDL_WindowFlags::SDL_WINDOW_HIDDEN)                           \
        .value("SDL_WINDOW_BORDERLESS", SDL_WindowFlags::SDL_WINDOW_BORDERLESS)                   \
        .value("SDL_WINDOW_RESIZABLE", SDL_WindowFlags::SDL_WINDOW_RESIZABLE)                     \
        .value("SDL_WINDOW_MINIMIZED", SDL_WindowFlags::SDL_WINDOW_MINIMIZED)                     \
        .value("SDL_WINDOW_MAXIMIZED", SDL_WindowFlags::SDL_WINDOW_MAXIMIZED)                     \
        .value("SDL_WINDOW_INPUT_GRABBED", SDL_WindowFlags::SDL_WINDOW_INPUT_GRABBED)             \
        .value("SDL_WINDOW_INPUT_FOCUS", SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS)                 \
        .value("SDL_WINDOW_MOUSE_FOCUS", SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS)                 \
        .value("SDL_WINDOW_FULLSCREEN_DESKTOP", SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP)   \
        .value("SDL_WINDOW_FOREIGN", SDL_WindowFlags::SDL_WINDOW_FOREIGN)                         \
        .value("SDL_WINDOW_ALLOW_HIGHDPI", SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI)             \
        .value("SDL_WINDOW_MOUSE_CAPTURE", SDL_WindowFlags::SDL_WINDOW_MOUSE_CAPTURE)             \
        .value("SDL_WINDOW_ALWAYS_ON_TOP", SDL_WindowFlags::SDL_WINDOW_ALWAYS_ON_TOP)             \
        .value("SDL_WINDOW_SKIP_TASKBAR", SDL_WindowFlags::SDL_WINDOW_SKIP_TASKBAR)               \
        .value("SDL_WINDOW_UTILITY", SDL_WindowFlags::SDL_WINDOW_UTILITY)                         \
        .value("SDL_WINDOW_TOOLTIP", SDL_WindowFlags::SDL_WINDOW_TOOLTIP)                         \
        .value("SDL_WINDOW_POPUP_MENU", SDL_WindowFlags::SDL_WINDOW_POPUP_MENU)                   \
        .value("SDL_WINDOW_VULKAN", SDL_WindowFlags::SDL_WINDOW_VULKAN);                          \

#endif