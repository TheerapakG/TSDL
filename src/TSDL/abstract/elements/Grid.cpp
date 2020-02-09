#include "TSDL/abstract/elements/Grid.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::Grid::Grid(TSDL_Renderer& renderer, const point_2d& size): Grid(renderer, size, ListenerMap()) {}

TSDL::elements::Grid::Grid(TSDL_Renderer& renderer, const point_2d& size , const ListenerMap& listeners): 
    Element(renderer), ElementHolder(renderer, size), EventDispatcher(renderer, listeners)
{
    Element::add_event_handler(
        TSDL::events::EventType::MouseMotion, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            point_2d _dist = caller.second;

            for(const Subelement& subelement: TSDL::reverse(get_child_order()))
            {
                auto& [element_ptr, dim] = subelement;
                auto& [topleft, bottomright] = dim;

                Element* element = element_ptr;

                if(topleft.x < _dist.x && _dist.x < bottomright.x &&
                    topleft.y < _dist.y && _dist.y < bottomright.y)
                    {
                        if(!_current_mouse_focus.has_value())
                        {
                            element->dispatch_event(Caller(*this, _dist - topleft), TSDL::events::EventType::MouseIn, event);
                        }
                        else if(element == _current_mouse_focus.value().element)
                        {
                            element->dispatch_event(Caller(*this, _dist - topleft), TSDL::events::EventType::MouseMotion, event);
                        }
                        else
                        {
                            Subelement _actual_focus = _current_mouse_focus.value();
                            _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::MouseOut, event);
                            _current_mouse_focus = subelement;
                            element->dispatch_event(Caller(*this, _dist - topleft), TSDL::events::EventType::MouseIn, event);
                        }
                        return false;
                    }
            }
            if(_current_mouse_focus.has_value())
            {
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::MouseOut, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::EventType::LeftDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::LeftDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::EventType::LeftUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::LeftUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::EventType::RightDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::RightDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::EventType::RightUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::RightUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::EventType::MiddleDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::MiddleDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::EventType::MiddleUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.element->dispatch_event(Caller(*this, _dist - _actual_focus.dimension.first), TSDL::events::EventType::MiddleUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
}