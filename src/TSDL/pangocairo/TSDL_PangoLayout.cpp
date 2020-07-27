//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/pangocairo/TSDL_PangoLayout.hpp"
#include "TSDL/TSDL_Main.hpp"

#include "TSDL/TSDL_Texture.hpp"
#include "TSDL/TSDL_Font.hpp"
#include "TSDL/pangocairo/TSDL_CairoSurface.hpp"
#include "TSDL/pangocairo/TSDL_CairoContext.hpp"

TSDL::TSDL_PangoLayout::TSDL_PangoLayout(TSDL_PangoLayout&& other): 
    _internal_ptr(other._internal_ptr), 
    _destroy(other._destroy)
{
    other._internal_ptr = nullptr;
}

TSDL::TSDL_PangoLayout::TSDL_PangoLayout(SDL_Type* ptr, bool destroy):
    _internal_ptr(ptr), _destroy(destroy) {}

TSDL::TSDL_PangoLayout::TSDL_PangoLayout(): 
    TSDL_PangoLayout(pango_layout_new(default_pango_context), true) {}

TSDL::TSDL_PangoLayout::~TSDL_PangoLayout()
{
    if(_destroy && _internal_ptr != nullptr) g_object_unref(_internal_ptr);
}

TSDL::TSDL_PangoLayout::operator PangoLayout*() const
{
    return _internal_ptr;
}

TSDL::TSDL_PangoLayout&  TSDL::TSDL_PangoLayout::text(const std::_TSDL_U8(string)& str)
{
    pango_layout_set_text(_internal_ptr, reinterpret_cast<const char*>(str.c_str()), -1);
    return *this;
}

std::_TSDL_U8(string) TSDL::TSDL_PangoLayout::text()
{
    return (const _TSDL_char*)(pango_layout_get_text(_internal_ptr)); //eww
}

TSDL::point_2d TSDL::TSDL_PangoLayout::size()
{
    point_2d ret;
    pango_layout_get_pixel_size(_internal_ptr, &ret.x, &ret.y);
    return ret;
}
        
TSDL::TSDL_PangoLayout&  TSDL::TSDL_PangoLayout::width(int w)
{
    pango_layout_set_width(_internal_ptr, w);
    return *this;
}

TSDL::TSDL_PangoLayout&  TSDL::TSDL_PangoLayout::height(int h)
{
    pango_layout_set_height(_internal_ptr, h);
    return *this;
}

TSDL::TSDL_PangoLayout&  TSDL::TSDL_PangoLayout::font(const TSDL_Font& font)
{
    pango_layout_set_font_description(_internal_ptr, font);
    return *this;
}

void TSDL::TSDL_PangoLayout::render_context_untidy(TSDL_CairoContext& context, const premul_color_rgba& c)
{
    render_context_untidy(context, c.r, c.g, c.b, c.a);
}

void TSDL::TSDL_PangoLayout::render_context_untidy(TSDL_CairoContext& context, double r, double g, double b, double a)
{
    context.render_color(r, g, b, a);
    pango_cairo_show_layout(context, _internal_ptr);
}

void TSDL::TSDL_PangoLayout::render_context(TSDL_CairoContext& context, const premul_color_rgba& c)
{
    render_context(context, c.r, c.g, c.b, c.a);
}

void TSDL::TSDL_PangoLayout::render_context(TSDL_CairoContext& context, double r, double g, double b, double a)
{
    cairo_pattern_t* source_pattern = cairo_get_source(context);
    render_context_untidy(context, r, g, b, a);
    cairo_set_source(context, source_pattern);
}

TSDL::TSDL_Texture TSDL::TSDL_PangoLayout::rendered_texture(TSDL_Renderer& renderer, const premul_color_rgba& c)
{
    return rendered_texture(renderer, c.r, c.g, c.b, c.a);
}

TSDL::TSDL_Texture TSDL::TSDL_PangoLayout::rendered_texture(TSDL_Renderer& renderer, double r, double g, double b, double a)
{
    auto _size = size();
    TSDL_Texture _ret(renderer, _size, SDL_TEXTUREACCESS_STREAMING, SDL_PIXELFORMAT_ARGB32);

    TSDL_CairoSurface _cairo_surface(_ret);
    TSDL_CairoContext _cairo_context(_cairo_surface);

    render_context_untidy(_cairo_context, r, g, b, a);
    _ret.blend_mode(SDL_BLENDMODE_BLEND);
    return _ret;
}