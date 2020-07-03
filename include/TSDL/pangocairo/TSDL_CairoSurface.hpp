//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_CAIROSURFACE_
#define TSDL_CAIROSURFACE_

#include <pango/pangocairo.h>
#include <tuple>
#include <utility>
#include <functional>

#include "TSDL/TSDL_Macro.hpp"

namespace TSDL
{
    class TSDL_Texture;
}

namespace TSDL
{
    class TSDL_CairoSurface
    {
        private:
        cairo_surface_t* _internal_ptr = nullptr;
        cairo_t* _context_ptr = nullptr;
        bool _destroy;
        std::function<void(void)> _destroy_cb = [](){};

        public:
        using SDL_Type = cairo_surface_t;

        TSDL_CairoSurface(TSDL_CairoSurface&& other);

        TSDL_CairoSurface(SDL_Type* ptr, bool destroy = false);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        TSDL_CairoSurface();
        TSDL_CairoSurface(TSDL_Texture& texture);

        ~TSDL_CairoSurface();

        operator cairo_surface_t*() const;
    };
}

#endif