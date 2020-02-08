#ifndef TSDL_ELEMENTS_ELEMENTHOLDER_
#define TSDL_ELEMENTS_ELEMENTHOLDER_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "abstract/elements/Element.hpp"

namespace TSDL
{
    namespace elements
    {
        using Subelement = std::pair<std::reference_wrapper<Element>, std::pair<point_2d, point_2d>>;
        class ElementHolder: virtual public Element
        {
            private:
            TSDL_Texture _rendered;
            std::vector <Subelement> _subelements_order;
            std::map <std::reference_wrapper<Element>, Subelement> _subelements_info;

            public:
            ElementHolder(TSDL_Renderer& renderer, const point_2d& size);

            void add_child(Element&& subelement, const point_2d& topleft, const point_2d& bottomright);
            void add_child(Element&& subelement, const point_2d& topleft, const point_2d& bottomright, int order);
            void reorder_child(Element&& subelement, int order);
            void move_child(Element&& subelement, const point_2d& destination);
            void remove_child(Element&& subelement);

            const std::vector<Subelement>& get_child_order();
            Subelement& highest_child(const point_2d& point);

            /*
            Re-render this element
            */
            virtual void render() override;

            /*
            Get previously rendered texture
            */
            virtual TSDL_Texture& get_texture() override;
        };
    }
}

#endif