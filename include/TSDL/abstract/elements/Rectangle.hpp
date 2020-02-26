#ifndef TSDL_ELEMENTS_RECTANGLE_
#define TSDL_ELEMENTS_RECTANGLE_

#include "TSDL/abstract/elements/Sized.hpp"

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        class Rectangle: public sized<RenderSizedElement>
        {
            private:
            color_rgba _color;

            public:
            Rectangle(const Rectangle& other);
            Rectangle(EventloopAdapter& evloop, const point_2d& size, const color_rgba& color = {0, 0, 0, 255});

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