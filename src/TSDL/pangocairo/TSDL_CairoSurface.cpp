//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/pangocairo/TSDL_CairoSurface.hpp"
#include "TSDL/TSDL_Texture.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include <SDL2/SDL.h>

TSDL::TSDL_CairoSurface::TSDL_CairoSurface(TSDL_CairoSurface&& other): 
    _internal_ptr(other._internal_ptr), 
    _destroy(other._destroy),
    _destroy_cb(std::move(other._destroy_cb))
{
    other._internal_ptr = nullptr;
}

TSDL::TSDL_CairoSurface::TSDL_CairoSurface(SDL_Type* ptr, bool destroy):
    _internal_ptr(ptr), _destroy(destroy) {}

TSDL::TSDL_CairoSurface::TSDL_CairoSurface(): 
    TSDL_CairoSurface(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0), true) {}

TSDL::TSDL_CairoSurface::TSDL_CairoSurface(TSDL_Texture& texture): 
    _destroy(true), 
    _destroy_cb([&texture](){SDL_UnlockTexture(texture);})
{
    // TODO: premultiply alpha?
    Uint32 format;
    int access, w, h, pitch;
    void* pixels;

    SDL_QueryTexture(texture, &format, &access, &w, &h);
    if (format != SDL_PIXELFORMAT_ARGB32)
    {
        TSDL::safe_throw<std::runtime_error>("Texture format is not ARGB32");
        return;
    }

    int _t = SDL_LockTexture(texture, NULL, &pixels, &pitch);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not lock texture! SDL Error: " + std::string(SDL_GetError()));
        return;
    }

    cairo_surface_t* _t_internal_ptr = cairo_image_surface_create_for_data(
        static_cast<unsigned char*>(pixels), 
        CAIRO_FORMAT_ARGB32,
        w, h, pitch
    );
    cairo_status_t _t_status = cairo_surface_status(_t_internal_ptr);
    if(_t_status != CAIRO_STATUS_SUCCESS)
    {
        TSDL::safe_throw<std::runtime_error>("Could make cairo surface! Cairo Error: " + std::string(cairo_status_to_string(_t_status)));
        return;
    }

    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_CairoSurface::~TSDL_CairoSurface()
{
    if(_destroy && _internal_ptr != nullptr)
    {
        cairo_surface_flush(_internal_ptr);
        cairo_surface_destroy(_internal_ptr);
        _destroy_cb();
    }
}

TSDL::TSDL_CairoSurface::operator cairo_surface_t*() const
{
    return _internal_ptr;
}