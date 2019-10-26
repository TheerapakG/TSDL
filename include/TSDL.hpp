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
    #include "TSDL/TSDL_Chunk.hpp"
    #include "TSDL/TSDL_Music.hpp"

    #ifdef TSDL_EXPOSE_PYBIND11
        #include "TSDL/TSDL_PY_Constants.hpp"
        PYBIND11_MODULE(tsdl, m)
        {
            _tsdl_py_constant_py(m);
            _tsdl_main_py(m);
            _tsdl_surface_py(m);
            _tsdl_texture_py(m);
            _tsdl_window_py(m);
            _tsdl_chunk_py(m);
            _tsdl_music_py(m);
        }
    #endif

#endif