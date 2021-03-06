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

#include "TSDL/Macro.hpp"

namespace TSDL
{
    class Texture;
}

namespace TSDL
{
    class CairoSurface
    {
        private:
        cairo_surface_t* _internal_ptr = nullptr;
        bool _destroy;
        std::function<void(void)> _destroy_cb = [](){};

        public:
        using SDL_Type = cairo_surface_t;

        CairoSurface(CairoSurface&& other);

        CairoSurface(SDL_Type* ptr, bool destroy = false);

        /*
        If exceptions is disabled, use TSDL::check_integrity to check
        if the object creation resulted in an error or not
        */
        CairoSurface();
        CairoSurface(Texture& texture);

        ~CairoSurface();

        operator cairo_surface_t*() const;
    };
}

#endif