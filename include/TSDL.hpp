#ifndef TSDL_H_
    #define TSDL_H_

    #ifdef TSDL_EXPOSE_PYBIND11
        #include "pybind11/pybind11.h"
        namespace py = pybind11;
    #endif

    #include "TSDL/TSDL_Eventloop.hpp"
    #include "TSDL/TSDL_Main.hpp"
    #include "TSDL/TSDL_Renderer.hpp"
    #include "TSDL/TSDL_Surface.hpp"
    #include "TSDL/TSDL_Texture.hpp"
    #include "TSDL/TSDL_Window.hpp"

    #ifdef TSDL_EXPOSE_PYBIND11
        #include "TSDL/TSDL_PY_Constants.hpp"
        PYBIND11_MODULE(tsdl, m)
        {
            _TSDL_PY_CONSTANTS_PY
            _TSDL_MAIN_PY
            _TSDL_SURFACE_PY
            _TSDL_TEXTURE_PY
            _TSDL_WINDOW_PY
        }
    #endif

#endif