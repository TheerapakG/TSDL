//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/pangocairo/CairoContext.hpp"
#include "TSDL/pangocairo/CairoSurface.hpp"
#include "TSDL/Utility.hpp"
#include <SDL2/SDL.h>

TSDL::CairoContext::CairoContext(CairoContext&& other): 
    _internal_ptr(other._internal_ptr), 
    _destroy(other._destroy)
{
    other._internal_ptr = nullptr;
}

TSDL::CairoContext::CairoContext(SDL_Type* ptr, bool destroy):
    _internal_ptr(ptr), _destroy(destroy) {}

TSDL::CairoContext::CairoContext(CairoSurface& surface): 
    _internal_ptr(cairo_create(surface)), _destroy(true) {}

TSDL::CairoContext::~CairoContext()
{
    if(_destroy && _internal_ptr != nullptr)
    {
        cairo_destroy(_internal_ptr);
    }
}

TSDL::CairoContext::operator cairo_t*() const
{
    return _internal_ptr;
}

void TSDL::CairoContext::render_color(const premul_color_rgba& color)
{
    render_color(color.r, color.g, color.b, color.a);
}

void TSDL::CairoContext::render_color(double r, double g, double b, double a)
{
    cairo_set_source_rgba(_internal_ptr, r, g, b, a);
}
