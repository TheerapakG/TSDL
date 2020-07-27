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

#include "TSDL/TSDL_Compat.hpp"
#include "TSDL/TSDL_Macro.hpp"
#include "TSDL/TSDL_Utility.hpp"

namespace TSDL
{
    class TSDL_Font;
    class TSDL_Texture;
    class TSDL_Renderer;
    class TSDL_CairoContext;
}

namespace TSDL
{
    class TSDL_PangoLayout
    {
        private:
        PangoLayout* _internal_ptr = nullptr;
        bool _destroy;

        public:
        using SDL_Type = PangoLayout;

        TSDL_PangoLayout(TSDL_PangoLayout&& other);

        TSDL_PangoLayout(SDL_Type* ptr, bool destroy = false);
        
        TSDL_PangoLayout();

        ~TSDL_PangoLayout();

        operator PangoLayout*() const;

        TSDL_PangoLayout& text(const std::_TSDL_U8(string)& str);
        std::_TSDL_U8(string) text();

        /*
        return size that would be rendered
        */
        point_2d size();

        TSDL_PangoLayout& width(int w);
        TSDL_PangoLayout& height(int h);

        TSDL_PangoLayout& font(const TSDL_Font& font);

        /*
        render the layout in the specified color to the context
        untidy variant do not reset context's source
        */
        void render_context_untidy(TSDL_CairoContext& context, const premul_color_rgba& c);
        void render_context_untidy(TSDL_CairoContext& context, double r, double g, double b, double a);
        void render_context(TSDL_CairoContext& context, const premul_color_rgba& c);
        void render_context(TSDL_CairoContext& context, double r, double g, double b, double a);

        /*
        return texture with the layout rendered in the specified color on the transparent background
        returned texture's blend mode is SDL_BLENDMODE_BLEND
        */
        TSDL_Texture rendered_texture(TSDL_Renderer& renderer, const premul_color_rgba& c);
        TSDL_Texture rendered_texture(TSDL_Renderer& renderer, double r, double g, double b, double a);
    };
}

#endif