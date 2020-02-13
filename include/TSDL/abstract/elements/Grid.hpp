#ifndef TSDL_ELEMENTS_GRID_
#define TSDL_ELEMENTS_GRID_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/ElementHolder.hpp"
#include "TSDL/abstract/elements/EventDispatcher.hpp"

namespace TSDL
{
    namespace elements
    {
        class Grid: public eventdispatcher<Element>, public ElementHolder
        {
            private:
            std::optional <Subelement> _current_mouse_focus;
            std::optional <Subelement> _left_origin, _right_origin, _middle_origin;

            public:
            Grid(TSDL_Renderer& renderer, const point_2d& size); 
            Grid(TSDL_Renderer& renderer, const point_2d& size, const ListenerMap& listeners);

            using ElementHolder::need_update;
            using ElementHolder::render;
            using eventdispatcher<Element>::dispatch_event;
        };
    }
}

#endif