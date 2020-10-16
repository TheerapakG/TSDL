//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ELEMENTHOLDER_
#define TSDL_ELEMENTS_ELEMENTHOLDER_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/attrs/Sizable.hpp"
#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/attrs/EventLookup.hpp"
#include "TSDL/Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        class Element_Traits
        {
            DependentElement* _dependent_ptr = nullptr;
            attrs::EventLookupable* _lookupable_ptr = nullptr;
            attrs::Sized* _sized_ptr = nullptr;

            public:
            constexpr Element_Traits(const Element_Traits&) = default;

            template <typename T>
            constexpr Element_Traits(T* element)
            {
                if constexpr (std::is_base_of_v<DependentElement, T>) _dependent_ptr = static_cast<DependentElement*>(element);
                if constexpr (std::is_base_of_v<attrs::EventLookupable, T>) _lookupable_ptr = static_cast<attrs::EventLookupable*>(element);
                if constexpr (std::is_base_of_v<attrs::Sized, T>) _sized_ptr = static_cast<attrs::Sized*>(element);
            }

            operator DependentElement* () const;
            operator attrs::EventLookupable* () const;
            operator attrs::Sized* () const;
        };

        struct Subelement
        {
            Element_Traits element;
            std::pair<point_2d, point_2d> dimension;

            bool operator==(const Subelement& other);
        };

        class ElementHolder: public attrs::sized<DependentElement>
        {
            public:
            using Subelement_vector = std::vector <Subelement>;

            private:
            Subelement_vector _subelements_order;
            std::map <DependentElement*, Subelement> _subelements_info;
            rect _render_position = {{0, 0}, {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()}};

            public:
            ElementHolder();

            virtual void add_child(const Subelement& formed_subelement);
            virtual Subelement_vector::iterator add_child(const Subelement& formed_subelement, int order);

            template<typename T, typename U =
                std::enable_if_t<
                std::is_base_of_v<attrs::EventLookupable, T> &&
                std::is_base_of_v<DependentElement, T>
            >>
            void add_child(T& subelement, const point_2d& topleft, const point_2d& bottomright)
            {
                std::pair el_loc(topleft, bottomright);
                Subelement el_all{&subelement, el_loc};

                add_child(el_all);
            }

            template<typename T, typename U = 
                std::enable_if_t<
                    std::is_base_of_v<attrs::EventLookupable, T>&&
                    std::is_base_of_v<DependentElement, T>
            >>
            Subelement_vector::iterator add_child(T& subelement, const point_2d& topleft, const point_2d& bottomright, int order)
            {
                std::pair el_loc(topleft, bottomright);
                Subelement el_all{&subelement, el_loc};

                return add_child(el_all, order);
            }

            template<typename T, typename U = std::enable_if_t<std::is_base_of_v<attrs::Sized, T>>> 
            void add_child(T& subelement, const point_2d& topleft)
            {
                std::pair el_loc(topleft, point_2d{0, 0});
                Subelement el_all{&subelement, el_loc};

                add_child(el_all);
            }

            template<typename T, typename U = std::enable_if_t<std::is_base_of_v<attrs::Sized, T>>>
            Subelement_vector::iterator add_child(T& subelement, const point_2d& topleft, int order)
            {
                std::pair el_loc(topleft, point_2d{0, 0});
                Subelement el_all{&subelement, el_loc};

                return add_child(el_all, order);
            }

            virtual Subelement_vector::iterator reorder_child(DependentElement& subelement, int order);
            virtual void move_child(DependentElement& subelement, const point_2d& destination);
            virtual void remove_child(DependentElement& subelement);

            Subelement child_info(DependentElement& subelement);
            rect bound() const;
            virtual point_2d size() const override;

            const std::vector<Subelement>& get_child_order();
            std::optional<Subelement> highest_child(const point_2d& point);

            void render_position(const rect& position);
            rect render_position();

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const override;

            /*
            Re-render this element
            */
            virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) override;
        };

        bool operator==(const ElementHolder& lhs, const ElementHolder& rhs);
    }
}

#endif