#include "TSDL/abstract/elements/Grid.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

void TSDL::elements::Grid::_init()
{
    Element::add_event_handler(
        ::TSDL::events::EventType::MouseMotion, 
        [this](const Caller& caller, const SDL_Event& event) -> bool
        {
            point_2d _dist = caller.second + render_position().topleft();
            _mouse_location = _dist;
            _last_mousemotion_event = event;

            if(_left_origin.has_value())
            {
                DependentElement& _actual_origin = _left_origin.value();
                _actual_origin.dispatch_event(
                    Caller(*this, _dist - child_info(_actual_origin).dimension.first), 
                    ::TSDL::events::EventType::MouseMotion, event
                );
                return false;
            }
            if(_middle_origin.has_value())
            {
                DependentElement& _actual_origin = _left_origin.value();
                _actual_origin.dispatch_event(
                    Caller(*this, _dist - child_info(_actual_origin).dimension.first), 
                    ::TSDL::events::EventType::MouseMotion, event
                );
                return false;
            }
            if(_right_origin.has_value())
            {
                DependentElement& _actual_origin = _left_origin.value();
                _actual_origin.dispatch_event(
                    Caller(*this, _dist - child_info(_actual_origin).dimension.first), 
                    ::TSDL::events::EventType::MouseMotion, event
                );
                return false;
            }

            for(const Subelement& subelement: ::TSDL::reverse(get_child_order()))
            {
                auto& [element_ptr, dim, sizable] = subelement;
                auto [topleft, bottomright] = dim;

                if(sizable != nullptr) bottomright = topleft + sizable->size();

                DependentElement* element = element_ptr;

                if (topleft.x < _dist.x && _dist.x < bottomright.x &&
                    topleft.y < _dist.y && _dist.y < bottomright.y)
                {
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
                        DependentElement& _actual_focus = _current_mouse_focus.value();
                        _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::MouseOut, event);
                        _current_mouse_focus = *element;
                        element->dispatch_event(Caller(*this, _dist - topleft), ::TSDL::events::EventType::MouseIn, event);
                    }
                    return false;
                }
            }

            if(_current_mouse_focus.has_value())
            {
                DependentElement& _actual_focus = _current_mouse_focus.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_focus = _current_mouse_focus.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::LeftUp, event);
                if(_left_origin.has_value())
                {
                    DependentElement& _actual_origin = _left_origin.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_origin = _left_origin.value();
                _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::LeftUp_Outside, event);
                _left_origin.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::LeftUp_Outside,
        [this](const Caller& caller, const SDL_Event& event) -> bool
        { 
            if(_left_origin.has_value())
            {
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_origin = _left_origin.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_focus = _current_mouse_focus.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::RightUp, event);
                if(_right_origin.has_value())
                {
                    DependentElement& _actual_origin = _right_origin.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_origin = _right_origin.value();
                _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::RightUp_Outside, event);
                _right_origin.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::RightUp_Outside,
        [this](const Caller& caller, const SDL_Event& event) -> bool
        { 
            if(_right_origin.has_value())
            {
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_origin = _right_origin.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_focus = _current_mouse_focus.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_focus = _current_mouse_focus.value();
                _actual_focus.dispatch_event(Caller(*this, _dist - child_info(_actual_focus).dimension.first), ::TSDL::events::EventType::MiddleUp, event);
                if(_middle_origin.has_value())
                {
                    DependentElement& _actual_origin = _middle_origin.value();
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
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_origin = _middle_origin.value();
                _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::MiddleUp_Outside, event);
                _middle_origin.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        ::TSDL::events::EventType::MiddleUp_Outside,
        [this](const Caller& caller, const SDL_Event& event) -> bool
        { 
            if(_middle_origin.has_value())
            {
                point_2d _dist = caller.second + render_position().topleft();
                DependentElement& _actual_origin = _middle_origin.value();
                _actual_origin.dispatch_event(Caller(*this, _dist - child_info(_actual_origin).dimension.first), ::TSDL::events::EventType::MiddleUp_Outside, event);
                _middle_origin.reset();
            }
            return false;
        }
    );
}

TSDL::elements::Grid::Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer): Grid(evloop, renderer, attrs::ListenerMap()) {}

TSDL::elements::Grid::Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, const attrs::ListenerMap& listeners): 
    attrs::eventdispatcher<ElementHolder>(evloop, renderer, listeners)
{
    _init();
}

TSDL::elements::Grid::Grid(EventloopAdapter& evloop, TSDL_Renderer& renderer, attrs::ListenerMap&& listeners): 
    attrs::eventdispatcher<ElementHolder>(evloop, renderer, listeners)
{
    _init();
}

void TSDL::elements::Grid::add_child(const Subelement& formed_subelement)
{
    attrs::eventdispatcher<ElementHolder>::add_child(formed_subelement);
    if (_left_origin.has_value()||_middle_origin.has_value()||_right_origin.has_value()) return;

    auto& [element_ptr, dim, sizable] = formed_subelement;
    auto [topleft, bottomright] = dim;
    if(sizable != nullptr) bottomright = topleft + sizable->size();

    if (topleft.x < _mouse_location.x && _mouse_location.x < bottomright.x &&
        topleft.y < _mouse_location.y && _mouse_location.y < bottomright.y)
    {
        _current_mouse_focus = *element_ptr;
        element_ptr->dispatch_event(
            Caller(*this, _mouse_location - topleft), 
            ::TSDL::events::EventType::MouseIn, _last_mousemotion_event
        );
    }
}

TSDL::elements::Grid::Subelement_vector::iterator TSDL::elements::Grid::add_child(const Subelement& formed_subelement, int order)
{
    auto it = attrs::eventdispatcher<ElementHolder>::add_child(formed_subelement, order);
    if(std::next(it)!=get_child_order().end()) return it;
    if(_left_origin.has_value()||_middle_origin.has_value()||_right_origin.has_value()) return it;

    auto& [element_ptr, dim, sizable] = formed_subelement;
    auto [topleft, bottomright] = dim;
    if(sizable != nullptr) bottomright = topleft + sizable->size();

    if (topleft.x < _mouse_location.x && _mouse_location.x < bottomright.x &&
        topleft.y < _mouse_location.y && _mouse_location.y < bottomright.y)
    {
        _current_mouse_focus = *element_ptr;
        element_ptr->dispatch_event(
            Caller(*this, _mouse_location - topleft), 
            ::TSDL::events::EventType::MouseIn, _last_mousemotion_event
        );
    }

    return it;
}

void TSDL::elements::Grid::remove_child(DependentElement& element)
{
    if(_current_mouse_focus.has_value())
    {
        if(_current_mouse_focus.value()==element) _current_mouse_focus.reset();
    }
    if(_left_origin.has_value())
    {
        if(_left_origin.value()==element) _left_origin.reset();
    }
    if(_middle_origin.has_value())
    {
        if(_middle_origin.value()==element) _middle_origin.reset();
    }
    if(_right_origin.has_value())
    {
        if(_right_origin.value()==element) _right_origin.reset();
    }
    attrs::eventdispatcher<ElementHolder>::remove_child(element);
}