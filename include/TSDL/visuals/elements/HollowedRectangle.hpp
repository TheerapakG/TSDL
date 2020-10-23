//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_HOLLOWEDRECTANGLE_
#define TSDL_ELEMENTS_HOLLOWEDRECTANGLE_

#include "TSDL/visuals/elements/attrs/Sizable.hpp"

#include "TSDL/Meta.hpp"

namespace TSDL::elements
{
    class HollowedRectangle: public attrs::sizable<RenderSizedElement>
    {
        private:
        color_rgba _color;
        int _thickness;

        public:
        HollowedRectangle(const HollowedRectangle& other);
        HollowedRectangle(const point_2d& size, const color_rgba& color = {0, 0, 0, 255}, int thickness = 1);

        /*
        Re-render this element
        */
        virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) override;

        /*
        Forces this element to be rendered with specified size
        */
        virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size) override;
    };
}

#endif