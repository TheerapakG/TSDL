#include "abstract/elements/Grid.hpp"
#include "TSDL_Meta.hpp"

TSDL::elements::Grid::Grid(TSDL_Renderer& renderer, const point_2d& size): Grid(renderer, size, ListenerMap()) {}

TSDL::elements::Grid::Grid(TSDL_Renderer& renderer, const point_2d& size , const ListenerMap& listeners): 
    Element(renderer), ElementHolder(renderer, size), EventDispatcher(renderer, listeners)
{
    Element::add_event_handler(
        TSDL::events::MouseMotion, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            point_2d _dist = caller.second;

            for(const Subelement& subelement: TSDL::reverse(get_child_order()))
            {
                auto& [element_ref, dim] = subelement;
                auto& [topleft, bottomright] = dim;

                Element& element = element_ref;

                if(topleft.x < _dist.x && _dist.x < bottomright.x &&
                    topleft.y < _dist.y && _dist.y < bottomright.y)
                    {
                        if(!_current_mouse_focus.has_value())
                        {
                            element.dispatch_event(Caller(*this, _dist - topleft), TSDL::events::MouseIn, event);
                        }
                        else if(element == _current_mouse_focus.value().first)
                        {
                            element.dispatch_event(Caller(*this, _dist - topleft), TSDL::events::MouseMotion, event);
                        }
                        else
                        {
                            Subelement _actual_focus = _current_mouse_focus.value();
                            _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::MouseOut, event);
                            _current_mouse_focus = subelement;
                            element.dispatch_event(Caller(*this, _dist - topleft), TSDL::events::MouseIn, event);
                        }
                        return false;
                    }
            }
            if(_current_mouse_focus.has_value())
            {
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::MouseOut, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::LeftDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::LeftDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::LeftUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::LeftUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::RightDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::RightDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::RightUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::RightUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::MiddleDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::MiddleDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::MiddleUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, _dist - _actual_focus.second.first), TSDL::events::MiddleUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
}