#ifndef TSDL_ELEMENTS_GRID_
#define TSDL_ELEMENTS_GRID_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include <limits>
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
            point_2d _mouse_location = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
            SDL_Event _last_mousemotion_event;

            void _init();

            public:
            Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer); 
            Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, const attrs::ListenerMap& listeners);
            Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, attrs::ListenerMap&& listeners);

            using attrs::eventdispatcher<ElementHolder>::add_child;
            virtual void add_child(const Subelement& formed_subelement) override;
            virtual Subelement_vector::iterator add_child(const Subelement& formed_subelement, int order) override;

            virtual void remove_child(DependentElement& element) override;
        };
    }
}

#endif