//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

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
    #include "TSDL/TSDL_Font.hpp"

    #include "TSDL/abstract/effects.hpp"
    #include "TSDL/abstract/elements.hpp"

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
            _tsdl_eventloop_py(m);
        }
    #endif

    #ifdef TSDL_USE_EMSCRIPTEN
        #include <emscripten.h>
    #endif

#endif