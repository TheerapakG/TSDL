//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/pangocairo/PangoLayout.hpp"
#include "TSDL/Main.hpp"

#include "TSDL/Texture.hpp"
#include "TSDL/Font.hpp"
#include "TSDL/pangocairo/CairoSurface.hpp"
#include "TSDL/pangocairo/CairoContext.hpp"

TSDL::PangoLayout::PangoLayout(PangoLayout&& other): 
    _internal_ptr(other._internal_ptr), 
    _destroy(other._destroy)
{
    other._internal_ptr = nullptr;
}

TSDL::PangoLayout::PangoLayout(SDL_Type* ptr, bool destroy):
    _internal_ptr(ptr), _destroy(destroy) {}

TSDL::PangoLayout::PangoLayout(): 
    PangoLayout(pango_layout_new(default_pango_context), true) {}

TSDL::PangoLayout::~PangoLayout()
{
    if(_destroy && _internal_ptr != nullptr) g_object_unref(_internal_ptr);
}

TSDL::PangoLayout::operator PangoLayout*() const
{
    return _internal_ptr;
}

TSDL::PangoLayout&  TSDL::PangoLayout::text(const std::_TSDL_U8(string)& str)
{
    pango_layout_set_text(_internal_ptr, reinterpret_cast<const char*>(str.c_str()), -1);
    return *this;
}

std::_TSDL_U8(string) TSDL::PangoLayout::text()
{
    return (const _char*)(pango_layout_get_text(_internal_ptr)); //eww
}

TSDL::point_2d TSDL::PangoLayout::size()
{
    point_2d ret;
    pango_layout_get_pixel_size(_internal_ptr, &ret.x, &ret.y);
    return ret;
}
        
TSDL::PangoLayout&  TSDL::PangoLayout::width(int w)
{
    pango_layout_set_width(_internal_ptr, w);
    return *this;
}

TSDL::PangoLayout&  TSDL::PangoLayout::height(int h)
{
    pango_layout_set_height(_internal_ptr, h);
    return *this;
}

TSDL::PangoLayout&  TSDL::PangoLayout::font(const Font& font)
{
    pango_layout_set_font_description(_internal_ptr, font);
    return *this;
}

void TSDL::PangoLayout::render_context_untidy(CairoContext& context, const premul_color_rgba& c)
{
    render_context_untidy(context, c.r, c.g, c.b, c.a);
}

void TSDL::PangoLayout::render_context_untidy(CairoContext& context, double r, double g, double b, double a)
{
    context.render_color(r, g, b, a);
    pango_cairo_show_layout(context, _internal_ptr);
}

void TSDL::PangoLayout::render_context(CairoContext& context, const premul_color_rgba& c)
{
    render_context(context, c.r, c.g, c.b, c.a);
}

void TSDL::PangoLayout::render_context(CairoContext& context, double r, double g, double b, double a)
{
    cairo_pattern_t* source_pattern = cairo_get_source(context);
    render_context_untidy(context, r, g, b, a);
    cairo_set_source(context, source_pattern);
}

TSDL::Texture TSDL::PangoLayout::rendered_texture(Renderer& renderer, const premul_color_rgba& c)
{
    return rendered_texture(renderer, c.r, c.g, c.b, c.a);
}

TSDL::Texture TSDL::PangoLayout::rendered_texture(Renderer& renderer, double r, double g, double b, double a)
{
    auto _size = size();
    Texture _ret(renderer, _size, SDL_TEXTUREACCESS_STREAMING, SDL_PIXELFORMAT_ARGB32);

    CairoSurface _cairo_surface(_ret);
    CairoContext _cairo_context(_cairo_surface);

    render_context_untidy(_cairo_context, r, g, b, a);
    _ret.blend_mode(SDL_BLENDMODE_BLEND);
    return _ret;
}