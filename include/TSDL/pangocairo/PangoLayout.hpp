//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_PANGOLAYOUT_
#define TSDL_PANGOLAYOUT_

#include <pango/pangocairo.h>
#include <tuple>
#include <utility>
#include <functional>

#include "TSDL/Compat.hpp"
#include "TSDL/Macro.hpp"
#include "TSDL/Utility.hpp"

namespace TSDL
{
    class Font;
    class Texture;
    class Renderer;
    class CairoContext;
}

namespace TSDL
{
    class PangoLayout
    {
        private:
        ::PangoLayout* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = ::PangoLayout;

        PangoLayout(PangoLayout&& other);

        PangoLayout(SDL_Type* ptr, bool destroy = false);
        
        PangoLayout();

        ~PangoLayout();

        operator SDL_Type*() const;

        PangoLayout& text(const std::_TSDL_U8(string)& str);
        std::_TSDL_U8(string) text();

        /*
        return size that would be rendered
        */
        point_2d size();

        PangoLayout& width(int w);
        PangoLayout& height(int h);

        PangoLayout& font(const Font& font);

        /*
        render the layout in the specified color to the context
        untidy variant do not reset context's source
        */
        void render_context_untidy(CairoContext& context, const premul_color_rgba& c);
        void render_context_untidy(CairoContext& context, double r, double g, double b, double a);
        void render_context(CairoContext& context, const premul_color_rgba& c);
        void render_context(CairoContext& context, double r, double g, double b, double a);

        /*
        return texture with the layout rendered in the specified color on the transparent background
        returned texture's blend mode is SDL_BLENDMODE_BLEND
        */
        Texture rendered_texture(Renderer& renderer, const premul_color_rgba& c);
        Texture rendered_texture(Renderer& renderer, double r, double g, double b, double a);
    };
}

#endif