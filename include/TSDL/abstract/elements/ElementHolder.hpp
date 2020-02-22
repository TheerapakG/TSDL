#ifndef TSDL_ELEMENTS_ELEMENTHOLDER_
#define TSDL_ELEMENTS_ELEMENTHOLDER_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/Sized.hpp"
#include "TSDL/abstract/elements/Element.hpp"

namespace TSDL
{
    namespace elements
    {
        struct Subelement
        {
            Element* element = nullptr;
            std::pair<point_2d, point_2d> dimension;

            Sized* _sized = nullptr;

            bool operator==(const Subelement& other);
        };

        class ElementHolder: virtual public Element
        {
            private:
            std::vector <Subelement> _subelements_order;
            std::map <Element*, Subelement> _subelements_info;

            public:
            ElementHolder(TSDL_Renderer& renderer);

            void add_child(Element& subelement, const point_2d& topleft, const point_2d& bottomright);
            void add_child(Element& subelement, const point_2d& topleft, const point_2d& bottomright, int order);

            template<typename T>
            void add_child(sized<T>& subelement, const point_2d& topleft)
            {
				std::pair<point_2d, point_2d> el_loc(topleft, {0, 0});
                Subelement el_all{&subelement, el_loc, &subelement};

                _subelements_order.push_back(el_all);
                _subelements_info[&subelement] = el_all;

                subelement._holders.emplace_back(*this);

                update();
            }

            template<typename T>
            void add_child(sized<T>& subelement, const point_2d& topleft, int order)
            {
                std::pair<point_2d, point_2d> el_loc(topleft, {0, 0});
                Subelement el_all{&subelement, el_loc, &subelement};

                auto it = _subelements_order.begin();
                _subelements_order.insert(it + order, el_all);
                _subelements_info[&subelement] = el_all;

                subelement._holders.emplace_back(*this);

                update();
            }

            void reorder_child(Element& subelement, int order);
            void move_child(Element& subelement, const point_2d& destination);
            void remove_child(Element& subelement);

            Subelement child_info(Element& subelement);

            const std::vector<Subelement>& get_child_order();
            std::optional<Subelement> highest_child(const point_2d& point);

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const override;

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;
        };

        bool operator==(const ElementHolder& lhs, const ElementHolder& rhs);
    }
}

#endif