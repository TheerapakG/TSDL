//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_CAIROCONTEXT_
#define TSDL_CAIROCONTEXT_

#include <pango/pangocairo.h>
#include <tuple>
#include <utility>
#include <functional>

#include "TSDL/TSDL_Macro.hpp"

namespace TSDL
{
    class TSDL_CairoSurface;
}

namespace TSDL
{
    class TSDL_CairoContext
    {
        private:
        cairo_t* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = cairo_t;

        TSDL_CairoContext(TSDL_CairoContext&& other);

        TSDL_CairoContext(SDL_Type* ptr, bool destroy = false);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_CairoContext(TSDL_CairoSurface& surface);

        ~TSDL_CairoContext();

        operator cairo_t*() const;

        void render_color(const premul_color_rgba& color);
        void render_color(double r, double g, double b, double a);
    };
}

#endif