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

    #include "TSDL/Eventloop.hpp"
    #include "TSDL/Main.hpp"
    #include "TSDL/Renderer.hpp"
    #include "TSDL/Surface.hpp"
    #include "TSDL/Texture.hpp"
    #include "TSDL/Window.hpp"
    #include "TSDL/Chunk.hpp"
    #include "TSDL/Music.hpp"
    #include "TSDL/Font.hpp"

    #include "TSDL/abstract/effects.hpp"
    #include "TSDL/abstract/elements.hpp"

    #ifdef TSDL_USE_PANGOCAIRO
        #include "TSDL/pangocairo/TSDL_CairoSurface.hpp"
        #include "TSDL/pangocairo/TSDL_PangoLayout.hpp"
    #endif

    #ifdef TSDL_EXPOSE_PYBIND11
        #include "TSDL/PY_Constants.hpp"
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