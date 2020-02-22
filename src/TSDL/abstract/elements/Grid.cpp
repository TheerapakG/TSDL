#include "TSDL/abstract/elements/Grid.hpp"

TSDL::elements::Grid::Grid(TSDL_Renderer& renderer): Grid(renderer, ListenerMap()) {}

TSDL::elements::Grid::Grid(TSDL_Renderer& renderer, const ListenerMap& listeners): 
    Element(renderer), ElementHolder(renderer), eventdispatcher<Element>(renderer, listeners)
{
    Element::add_event_handler(
        ::TSDL::events::EventType::MouseMotion, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            point_2d _dist = caller.second;

            for(const Subelement& subelement: ::TSDL::reverse(get_child_order()))
            {
                auto& [element_ptr, dim, sized] = subelement;
                auto [topleft, bottomright] = dim;

                if(sized != nullptr) bottomright = topleft + sized->size();

                Element* element = element_ptr;

                if(topleft.x < _dist.x && _dist.x < bottomright.x &&
                    topleft.y < _dist.y && _dist.y < bottomright.y)
                    {
                        if(_left_origin.has_value())
                        {
                            Element& _actual_origin = _left_origin.value();
                            if(*element != _actual_origin) _actual_origin.dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseMotion, event);
                        }
                        if(_middle_origin.has_value())
                        {
                            Element& _actual_origin = _left_origin.value();
                            if(*element != _actual_origin) _actual_origin.dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseMotion, event);
                        }
                        if(_right_origin.has_value())
                        {
                            Element& _actual_origin = _left_origin.value();
                            if(*element != _actual_origin) element->dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseMotion, event);
                        }

                        if(!_current_mouse_focus.has_value())
                        {
                            _current_mouse_focus = *element;
                            element->dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseIn, event);
                            element->dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseMotion, event);
                        }
                        else if(*element == _current_mouse_focus.value().get())
                        {
                            element->dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseMotion, event);
                        }
                        else
                        {
                            Element& _actual_focus = _current_mouse_focus.value();
                            _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::MouseOut, event);
                            _current_mouse_focus = *element;
                            element->dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseIn, event);
                        }
                        return false;
                    }
            }
            if(_current_mouse_focus.has_value())
            {
                Element& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::MouseOut, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::LeftDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                _left_origin = _current_mouse_focus;
                point_2d _dist = caller.second;
                Element& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::LeftDown, event);
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::LeftUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Element& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::LeftUp, event);
                if(_left_origin.has_value())
                {
                    Element& _actual_origin = _left_origin.value();
                    if(_actual_origin == _actual_focus)
                    {
                        _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::LeftUp_Inside, event);
                    }
                    else
                    {
                        _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::LeftUp_Outside, event);
                    }
                    _left_origin.reset();
                }
            }
            else if(_left_origin.has_value())
            {
                point_2d _dist = caller.second;
                Element& _actual_origin = _left_origin.value();
                _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::LeftUp_Outside, event);
                _left_origin.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::RightDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                _right_origin = _current_mouse_focus;
                point_2d _dist = caller.second;
                Element& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::RightDown, event);
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::RightUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Element& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::RightUp, event);
                if(_right_origin.has_value())
                {
                    Element& _actual_origin = _right_origin.value();
                    if(_actual_origin == _actual_focus)
                    {
                        _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::RightUp_Inside, event);
                    }
                    else
                    {
                        _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::RightUp_Outside, event);
                    }
                    _right_origin.reset();
                }
            }
            else if(_right_origin.has_value())
            {
                point_2d _dist = caller.second;
                Element& _actual_origin = _right_origin.value();
                _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::RightUp_Outside, event);
                _right_origin.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::MiddleDown, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                _middle_origin = _current_mouse_focus;
                point_2d _dist = caller.second;
                Element& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::MiddleDown, event);
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::MiddleUp, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _dist = caller.second;
                Element& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::MiddleUp, event);
                if(_middle_origin.has_value())
                {
                    Element& _actual_origin = _middle_origin.value();
                    if(_actual_origin == _actual_focus)
                    {
                        _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::MiddleUp_Inside, event);
                    }
                    else
                    {
                        _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::MiddleUp_Outside, event);
                    }
                    _middle_origin.reset();
                }
            }
            else if(_middle_origin.has_value())
            {
                point_2d _dist = caller.second;
                Element& _actual_origin = _middle_origin.value();
                _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::MiddleUp_Outside, event);
                _middle_origin.reset();
            }
            return false;
        }
    );
}