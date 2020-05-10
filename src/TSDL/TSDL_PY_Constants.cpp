//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/TSDL_PY_Constants.hpp"

#include "TSDL/TSDL_Utility.hpp"

#ifdef TSDL_EXPOSE_PYBIND11
void _tsdl_py_constant_py(const py::module& m)
{
    m.attr("SDL_WINDOWPOS_CENTERED") = SDL_WINDOWPOS_CENTERED;
    m.attr("SDL_WINDOWPOS_UNDEFINED") = SDL_WINDOWPOS_UNDEFINED;
    py::enum_<SDL_WindowFlags>(m, "SDL_WindowFlags")
        .value("SDL_WINDOW_FULLSCREEN", SDL_WindowFlags::SDL_WINDOW_FULLSCREEN)
        .value("SDL_WINDOW_OPENGL", SDL_WindowFlags::SDL_WINDOW_OPENGL)
        .value("SDL_WINDOW_SHOWN", SDL_WindowFlags::SDL_WINDOW_SHOWN)
        .value("SDL_WINDOW_HIDDEN", SDL_WindowFlags::SDL_WINDOW_HIDDEN)
        .value("SDL_WINDOW_BORDERLESS", SDL_WindowFlags::SDL_WINDOW_BORDERLESS)
        .value("SDL_WINDOW_RESIZABLE", SDL_WindowFlags::SDL_WINDOW_RESIZABLE)
        .value("SDL_WINDOW_MINIMIZED", SDL_WindowFlags::SDL_WINDOW_MINIMIZED)
        .value("SDL_WINDOW_MAXIMIZED", SDL_WindowFlags::SDL_WINDOW_MAXIMIZED)
        .value("SDL_WINDOW_INPUT_GRABBED", SDL_WindowFlags::SDL_WINDOW_INPUT_GRABBED)
        .value("SDL_WINDOW_INPUT_FOCUS", SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS)
        .value("SDL_WINDOW_MOUSE_FOCUS", SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS)
        .value("SDL_WINDOW_FULLSCREEN_DESKTOP", SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP)
        .value("SDL_WINDOW_FOREIGN", SDL_WindowFlags::SDL_WINDOW_FOREIGN)
        .value("SDL_WINDOW_ALLOW_HIGHDPI", SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI)
        .value("SDL_WINDOW_MOUSE_CAPTURE", SDL_WindowFlags::SDL_WINDOW_MOUSE_CAPTURE)
        .value("SDL_WINDOW_ALWAYS_ON_TOP", SDL_WindowFlags::SDL_WINDOW_ALWAYS_ON_TOP)
        .value("SDL_WINDOW_SKIP_TASKBAR", SDL_WindowFlags::SDL_WINDOW_SKIP_TASKBAR)
        .value("SDL_WINDOW_UTILITY", SDL_WindowFlags::SDL_WINDOW_UTILITY)
        .value("SDL_WINDOW_TOOLTIP", SDL_WindowFlags::SDL_WINDOW_TOOLTIP)
        .value("SDL_WINDOW_POPUP_MENU", SDL_WindowFlags::SDL_WINDOW_POPUP_MENU)
        .value("SDL_WINDOW_VULKAN", SDL_WindowFlags::SDL_WINDOW_VULKAN)
        .def("__or__", &::TSDL::or_enum<SDL_WindowFlags>);
    
    py::enum_<SDL_BlendMode>(m, "SDL_BlendMode")
        .value("SDL_BLENDMODE_NONE", SDL_BlendMode::SDL_BLENDMODE_NONE)
        .value("SDL_BLENDMODE_BLEND", SDL_BlendMode::SDL_BLENDMODE_BLEND)
        .value("SDL_BLENDMODE_ADD", SDL_BlendMode::SDL_BLENDMODE_ADD)
        .value("SDL_BLENDMODE_MOD", SDL_BlendMode::SDL_BLENDMODE_MOD);
    
    py::enum_<SDL_RendererFlags>(m, "SDL_RendererFlags")
        .value("SDL_RENDERER_SOFTWARE", SDL_RendererFlags::SDL_RENDERER_SOFTWARE)
        .value("SDL_RENDERER_ACCELERATED", SDL_RendererFlags::SDL_RENDERER_ACCELERATED)
        .value("SDL_RENDERER_PRESENTVSYNC", SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC)
        .value("SDL_RENDERER_TARGETTEXTURE", SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE)
        .def("__or__", &::TSDL::or_enum<SDL_RendererFlags>);
    
    py::enum_<SDL_RendererFlip>(m, "SDL_RendererFlip")
        .value("SDL_FLIP_NONE", SDL_RendererFlip::SDL_FLIP_NONE)
        .value("SDL_FLIP_HORIZONTAL", SDL_RendererFlip::SDL_FLIP_HORIZONTAL)
        .value("SDL_FLIP_VERTICAL", SDL_RendererFlip::SDL_FLIP_VERTICAL)
        .def("__or__", &::TSDL::or_enum<SDL_RendererFlip>);
    
    py::enum_<SDL_EventType>(m, "SDL_EventType")
        .value("SDL_FIRSTEVENT", SDL_EventType::SDL_FIRSTEVENT)
        .value("SDL_QUIT", SDL_EventType::SDL_QUIT)
        .value("SDL_APP_TERMINATING", SDL_EventType::SDL_APP_TERMINATING)
        .value("SDL_APP_LOWMEMORY", SDL_EventType::SDL_APP_LOWMEMORY)
        .value("SDL_APP_WILLENTERBACKGROUND", SDL_EventType::SDL_APP_WILLENTERBACKGROUND)
        .value("SDL_APP_DIDENTERBACKGROUND", SDL_EventType::SDL_APP_DIDENTERBACKGROUND)
        .value("SDL_APP_WILLENTERFOREGROUND", SDL_EventType::SDL_APP_WILLENTERFOREGROUND)
        .value("SDL_APP_DIDENTERFOREGROUND", SDL_EventType::SDL_APP_DIDENTERFOREGROUND)
        .value("SDL_DISPLAYEVENT", SDL_EventType::SDL_DISPLAYEVENT)
        .value("SDL_WINDOWEVENT", SDL_EventType::SDL_WINDOWEVENT)
        .value("SDL_SYSWMEVENT", SDL_EventType::SDL_SYSWMEVENT)
        .value("SDL_KEYDOWN", SDL_EventType::SDL_KEYDOWN)
        .value("SDL_KEYUP", SDL_EventType::SDL_KEYUP)
        .value("SDL_TEXTEDITING", SDL_EventType::SDL_TEXTEDITING)
        .value("SDL_TEXTINPUT", SDL_EventType::SDL_TEXTINPUT)
        .value("SDL_KEYMAPCHANGED", SDL_EventType::SDL_KEYMAPCHANGED)
        .value("SDL_MOUSEMOTION", SDL_EventType::SDL_MOUSEMOTION)
        .value("SDL_MOUSEBUTTONDOWN", SDL_EventType::SDL_MOUSEBUTTONDOWN)
        .value("SDL_MOUSEBUTTONUP", SDL_EventType::SDL_MOUSEBUTTONUP)
        .value("SDL_JOYAXISMOTION", SDL_EventType::SDL_JOYAXISMOTION)
        .value("SDL_JOYBALLMOTION", SDL_EventType::SDL_JOYBALLMOTION)
        .value("SDL_JOYHATMOTION", SDL_EventType::SDL_JOYHATMOTION)
        .value("SDL_JOYBUTTONDOWN", SDL_EventType::SDL_JOYBUTTONDOWN)
        .value("SDL_JOYBUTTONUP", SDL_EventType::SDL_JOYBUTTONUP)
        .value("SDL_JOYDEVICEADDED", SDL_EventType::SDL_JOYDEVICEADDED)
        .value("SDL_JOYDEVICEREMOVED", SDL_EventType::SDL_JOYDEVICEREMOVED)
        .value("SDL_CONTROLLERAXISMOTION", SDL_EventType::SDL_CONTROLLERAXISMOTION)
        .value("SDL_CONTROLLERBUTTONDOWN", SDL_EventType::SDL_CONTROLLERBUTTONDOWN)
        .value("SDL_CONTROLLERBUTTONUP", SDL_EventType::SDL_CONTROLLERBUTTONUP)
        .value("SDL_CONTROLLERDEVICEADDED", SDL_EventType::SDL_CONTROLLERDEVICEADDED)
        .value("SDL_CONTROLLERDEVICEREMOVED", SDL_EventType::SDL_CONTROLLERDEVICEREMOVED)
        .value("SDL_CONTROLLERDEVICEREMAPPED", SDL_EventType::SDL_CONTROLLERDEVICEREMAPPED)
        .value("SDL_FINGERDOWN", SDL_EventType::SDL_FINGERDOWN)
        .value("SDL_FINGERUP", SDL_EventType::SDL_FINGERUP)
        .value("SDL_FINGERMOTION", SDL_EventType::SDL_FINGERMOTION)
        .value("SDL_DOLLARGESTURE", SDL_EventType::SDL_DOLLARGESTURE)
        .value("SDL_DOLLARRECORD", SDL_EventType::SDL_DOLLARRECORD)
        .value("SDL_MULTIGESTURE", SDL_EventType::SDL_MULTIGESTURE)
        .value("SDL_CLIPBOARDUPDATE", SDL_EventType::SDL_CLIPBOARDUPDATE)
        .value("SDL_DROPFILE", SDL_EventType::SDL_DROPFILE)
        .value("SDL_DROPTEXT", SDL_EventType::SDL_DROPTEXT)
        .value("SDL_DROPBEGIN", SDL_EventType::SDL_DROPBEGIN)
        .value("SDL_DROPCOMPLETE", SDL_EventType::SDL_DROPCOMPLETE)
        .value("SDL_AUDIODEVICEADDED", SDL_EventType::SDL_AUDIODEVICEADDED)
        .value("SDL_AUDIODEVICEREMOVED", SDL_EventType::SDL_AUDIODEVICEREMOVED)
        .value("SDL_SENSORUPDATE", SDL_EventType::SDL_SENSORUPDATE)
        .value("SDL_RENDER_TARGETS_RESET", SDL_EventType::SDL_RENDER_TARGETS_RESET)
        .value("SDL_RENDER_DEVICE_RESET", SDL_EventType::SDL_RENDER_DEVICE_RESET)
        .value("SDL_USEREVENT", SDL_EventType::SDL_USEREVENT)
        .value("SDL_LASTEVENT", SDL_EventType::SDL_LASTEVENT);

    py::class_<SDL_Event>(m, "SDL_Event")
        .def_readwrite("type", &SDL_Event::type)
        .def_readwrite("common", &SDL_Event::common)
        .def_readwrite("display", &SDL_Event::display)
        .def_readwrite("window", &SDL_Event::window)
        .def_readwrite("key", &SDL_Event::key)
        .def_readwrite("edit", &SDL_Event::edit)
        .def_readwrite("text", &SDL_Event::text)
        .def_readwrite("motion", &SDL_Event::motion)
        .def_readwrite("button", &SDL_Event::button)
        .def_readwrite("wheel", &SDL_Event::wheel)
        .def_readwrite("jaxis", &SDL_Event::jaxis)
        .def_readwrite("jball", &SDL_Event::jball)
        .def_readwrite("jhat", &SDL_Event::jhat)
        .def_readwrite("jbutton", &SDL_Event::jbutton)
        .def_readwrite("jdevice", &SDL_Event::jdevice)
        .def_readwrite("caxis", &SDL_Event::caxis)
        .def_readwrite("cbutton", &SDL_Event::cbutton)
        .def_readwrite("cdevice", &SDL_Event::cdevice)
        .def_readwrite("adevice", &SDL_Event::adevice)
        .def_readwrite("sensor", &SDL_Event::sensor)
        .def_readwrite("quit", &SDL_Event::quit)
        .def_readwrite("user", &SDL_Event::user)
        .def_readwrite("syswm", &SDL_Event::syswm)
        .def_readwrite("tfinger", &SDL_Event::tfinger)
        .def_readwrite("mgesture", &SDL_Event::mgesture)
        .def_readwrite("dgesture", &SDL_Event::dgesture)
        .def_readwrite("drop", &SDL_Event::drop);
}
#endif