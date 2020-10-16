//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_TEXTUREELEMENT_
#define TSDL_ELEMENTS_TEXTUREELEMENT_

#include "TSDL/abstract/elements/attrs/Sizable.hpp"
#include <memory>

namespace TSDL
{
    namespace elements
    {
        class TextureElement: public attrs::sizable<RenderSizedElement>
        {
            private:
            std::shared_ptr<Texture> _texture;

            public:
            TextureElement(const TextureElement& other);
            TextureElement(const point_2d& size, std::shared_ptr<Texture> texture);

            Texture& texture();

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
}

#endif