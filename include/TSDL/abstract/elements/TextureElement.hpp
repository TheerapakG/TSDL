#ifndef TSDL_ELEMENTS_TEXTUREELEMENT_
#define TSDL_ELEMENTS_TEXTUREELEMENT_

#include "TSDL/abstract/elements/Sized.hpp"
#include <memory>

namespace TSDL
{
    namespace elements
    {
        class TextureElement: public sized<Element>
        {
            private:
            std::shared_ptr<TSDL_Texture> _texture;

            public:
            TextureElement(const TextureElement& other);
            TextureElement(TSDL_Renderer& renderer, const point_2d& size, std::shared_ptr<TSDL_Texture> texture);

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;

            /*
            Forces this element to be rendered with specified size
            */
            void render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size);
        };
    }
}

#endif