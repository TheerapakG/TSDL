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
        class ElementHolder
        {
            private:
            std::vector <Subelement> _subelements_order;
            std::map <std::reference_wrapper<Element>, Subelement> _subelements_info;

            public:
            void add_child(const Element& subelement, const point_2d& topleft, const point_2d& bottomright);
            void add_child(const Element& subelement, const point_2d& topleft, const point_2d& bottomright, int order);
            void reorder_child(const Element& subelement, int order);
            void move_child(const Element& subelement, const point_2d& destination);
            void remove_child(const Element& subelement);

            const std::vector<Subelement>& get_child_order();
            Subelement& highest_child(const point_2d& point);
        };
    }
}

#endif