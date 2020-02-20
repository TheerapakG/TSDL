#ifndef TSDL_ELEMENTS_ATTRS_DRAGABLE_
#define TSDL_ELEMENTS_ATTRS_DRAGABLE_

#include "TSDL/abstract/elements/Element.hpp"

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        namespace attrs
        {
            class Dragable
            {
                private:
                ::TSDL::point_2d _pos;
                transformer<::TSDL::point_2d> _pos_transform_fun;

                public:
                Dragable(transformer<::TSDL::point_2d> pos_transform, ::TSDL::point_2d origin = {0, 0});

                ::TSDL::point_2d pos();
                void pos(const ::TSDL::point_2d& new_pos);
            };

            template <class T>
            class dragable: public std::enable_if_t<std::is_base_of_v<Element, T>, T>, public Dragable
            {
                public:
                template <typename ...Args>
                dragable(
                    TSDL_Renderer& renderer, 
                    transformer<::TSDL::point_2d> pos_transform, 
                    Args... args
                ): T(renderer, args...), Dragable(pos_transform) 
                {

                }

                template <typename ...Args>
                dragable(
                    TSDL_Renderer& renderer, 
                    transformer<::TSDL::point_2d> pos_transform, 
                    ::TSDL::point_2d origin, 
                    Args... args
                ): T(renderer, args...), Dragable(pos_transform, origin) 
                {

                }
            };
        }
    }
}

#endif