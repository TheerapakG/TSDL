//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_MAIN_
#define TSDL_MAIN_

#include <SDL2/SDL.h>

#ifdef TSDL_USE_PANGOCAIRO
#include <pango/pangocairo.h>
#endif

#ifdef TSDL_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#ifdef TSDL_EXPOSE_PYBIND11
#include "PY_TypeErase.hpp"
_PY_EXPAND_DECLARE_CLASS(TSDL)
namespace _PY
{
    _PY_EXPAND_DECLARE_CONTEXTMANAGER(TSDL)
    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(TSDL)
    _PY_GET_CONTEXTMANAGER(TSDL)<>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE
}
#else
#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME)
#endif

namespace TSDL
{
    extern SDL_Event null_event;

    #ifdef TSDL_USE_PANGOCAIRO
    extern PangoContext* default_pango_context;
    #endif

    class TSDL
    {
        public:
        #ifndef __cpp_exceptions
        bool constructed = false;
        #endif

        _PY_DECLARE_TYPEERASE_OWNER(TSDL)

        TSDL();
        TSDL(int frequency);
        ~TSDL();
    };
}

#ifdef TSDL_EXPOSE_PYBIND11

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(TSDL)

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(TSDL)
}

void _tsdl_main_py(const py::module& m);


#endif

#endif