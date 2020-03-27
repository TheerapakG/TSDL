#ifndef TSDL_ELEMENTS_ATTRS_GRIDDED_
#define TSDL_ELEMENTS_ATTRS_GRIDDED_

#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/attrs/EventForwarder.hpp"
#include "TSDL/abstract/elements/Grid.hpp"

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL::elements::attrs
{
    template <class Grid_T, typename U = std::enable_if_t<std::is_base_of_v<Grid, Grid_T>>>
    class Gridded
    {
        private:
        Grid_T _grid;

        public:
        Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer): _grid(evloop, renderer) {}
        Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ListenerMap& listeners): _grid(evloop, renderer, listeners) {}
        Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer, ListenerMap&& listeners): _grid(evloop, renderer, listeners) {}

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
        gridded(
            EventloopAdapter& evloop, 
            TSDL_Renderer& renderer, 
            Args... args
        ): T(evloop, renderer, args...), Gridded<Grid_T>(evloop, renderer) {}

        template <typename ...Args>
        gridded(
            EventloopAdapter& evloop,
            TSDL_Renderer& renderer, 
            const ListenerMap& listeners,
            Args... args
        ): T(evloop, renderer, args...), Gridded<Grid_T>(evloop, renderer, listeners) {}

        template <typename ...Args>
        gridded(
            EventloopAdapter& evloop,
            TSDL_Renderer& renderer, 
            ListenerMap&& listeners,
            Args... args
        ): T(evloop, renderer, args...), Gridded<Grid_T>(evloop, renderer, listeners) {}

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
        virtual void render(const ::TSDL::point_2d& dist) override
        {
            grid().render(dist);
        }
    };
}

#endif