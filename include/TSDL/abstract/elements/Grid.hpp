#ifndef TSDL_ELEMENTS_GRID_
#define TSDL_ELEMENTS_GRID_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/ElementHolder.hpp"
#include "TSDL/abstract/elements/EventDispatcher.hpp"
#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        class Grid: public eventdispatcher<Element>, public ElementHolder
        {
            private:
            optional_reference<Element> _current_mouse_focus;
            optional_reference<Element> _left_origin, _right_origin, _middle_origin;

            public:
            Grid(TSDL_Renderer& renderer); 
            Grid(TSDL_Renderer& renderer, const ListenerMap& listeners);

            using ElementHolder::need_update;
            using ElementHolder::render;
            using eventdispatcher<Element>::dispatch_event;
        };
    }
}

#endif