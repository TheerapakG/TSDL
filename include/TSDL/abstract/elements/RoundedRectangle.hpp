#ifndef TSDL_ELEMENTS_ROUNDEDRECTANGLE_
#define TSDL_ELEMENTS_ROUNDEDRECTANGLE_

#include "TSDL/abstract/elements/attrs/Sizable.hpp"

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        class RoundedRectangle: public attrs::sizable<RenderSizedElement>
        {
            private:
            color_rgba _color;
            int _r;

            public:
            RoundedRectangle(const RoundedRectangle& other);
            RoundedRectangle(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, int r, const color_rgba& color = {0, 0, 0, 255});

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;

            /*
            Forces this element to be rendered with specified size
            */
            virtual void render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size) override;
        };
    }
}

#endif