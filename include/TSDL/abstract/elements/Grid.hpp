#ifndef TSDL_ELEMENTS_GRID_
#define TSDL_ELEMENTS_GRID_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "abstract/elements/ElementHolder.hpp"
#include "abstract/elements/EventDispatcher.hpp"

namespace TSDL
{
    namespace elements
    {
        using Subelement = std::pair<std::reference_wrapper<Element>, std::pair<point_2d, point_2d>>;
        class Grid: public EventDispatcher, public ElementHolder
        {
            private:
            std::optional <Subelement> _current_mouse_focus;

            public:
            Grid();
            Grid(const ListenerMap& listeners);

            /*
            Re-render this element
            */
            void render() final;

            /*
            Get previously rendered texture
            */
            TSDL_Texture& get_texture() final;
        };
    }
}

#endif