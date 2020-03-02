#ifndef TSDL_ELEMENTS_GRID_
#define TSDL_ELEMENTS_GRID_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/ElementHolder.hpp"
#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        class Grid: public attrs::eventdispatcher<ElementHolder>
        {
            private:
            optional_reference<DependentElement> _current_mouse_focus;
            optional_reference<DependentElement> _left_origin, _right_origin, _middle_origin;

            void _init();

            public:
            Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer); 
            Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, const attrs::ListenerMap& listeners);
            Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, attrs::ListenerMap&& listeners);
        };
    }
}

#endif