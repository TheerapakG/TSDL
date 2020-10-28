//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_ATTRS_GRIDDED_
#define TSDL_ELEMENTS_ATTRS_GRIDDED_

#include "TSDL/visuals/elements/Element.hpp"
#include "TSDL/visuals/elements/attrs/EventForwarder.hpp"
#include "TSDL/visuals/elements/Grid.hpp"

#include "TSDL/Meta.hpp"

namespace TSDL::elements::attrs
{
    template <class Grid_T, typename U = std::enable_if_t<std::is_base_of_v<Grid, Grid_T>>>
    class Gridded
    {
        private:
        Grid_T _grid;

        public:
        Gridded(): _grid() {}
        Gridded(const ListenerMap& listeners): _grid(listeners) {}
        Gridded(ListenerMap&& listeners): _grid(listeners) {}

        Grid_T& grid()
        {
            return _grid;
        }

        const Grid_T& grid() const
        {
            return _grid;
        }
    };

    template <class T, class Grid_T>
    class gridded: public std::enable_if_t<std::is_base_of_v<DependentElement, T>, T>, public Gridded<Grid_T>
    {
        public:
        template <typename ...Args>
        gridded(Args... args): T(args...), Gridded<Grid_T>() {}

        template <typename ...Args>
        gridded(
            const ListenerMap& listeners,
            Args... args
        ): T(args...), Gridded<Grid_T>(listeners) {}

        template <typename ...Args>
        gridded(
            ListenerMap&& listeners,
            Args... args
        ): T(args...), Gridded<Grid_T>(listeners) {}

        /*
        Query if parent need to update this element on the next cycle
        */
        virtual bool need_update() const override
        {
            return grid().need_update();
        }

        /*
        Re-render this element
        */
        virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) const override
        {
            grid().render(window, dist);
        }
    };
}

#endif