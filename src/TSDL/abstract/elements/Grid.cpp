#include "abstract/elements/Grid.hpp"
#include "TSDL_Meta.hpp"

TSDL::elements::Grid::Grid(): Grid(ListenerMap()) {}

TSDL::elements::Grid::Grid(const ListenerMap& listeners): EventDispatcher(listeners)
{
    Element::add_event_handler(
        TSDL::events::MouseMotion, 
        [this](const Caller& caller, const SDL_Event& event)
        {
            point_2d _this_dist = caller.second;
            int x = event.motion.x - _this_dist.x;
            int y = event.motion.y - _this_dist.y;

            for(const Subelement& subelement: TSDL::reverse(get_child_order()))
            {
                auto& [element_ref, dim] = subelement;
                auto& [topleft, bottomright] = dim;

                Element& element = element_ref;

                if(topleft.x < x && x < bottomright.x &&
                    topleft.y < y && y < bottomright.y)
                    {
                        if(!_current_mouse_focus.has_value())
                        {
                            element.dispatch_event(Caller(*this, {x - topleft.x, y - topleft.y}), TSDL::events::MouseIn, event);
                        }
                        else if(element == _current_mouse_focus.value().first)
                        {
                            element.dispatch_event(Caller(*this, {x - topleft.x, y - topleft.y}), TSDL::events::MouseMotion, event);
                        }
                        else
                        {
                            Subelement _actual_focus = _current_mouse_focus.value();
                            _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::MouseOut, event);
                            _current_mouse_focus = subelement;
                            element.dispatch_event(Caller(*this, {x - topleft.x, y - topleft.y}), TSDL::events::MouseIn, event);
                        }
                        return false;
                    }
            }
            if(_current_mouse_focus.has_value())
            {
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::MouseOut, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::LeftDown, 
        [this](const Caller& caller, const SDL_Event& event)
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _this_dist = caller.second;
                int x = event.button.x - _this_dist.x;
                int y = event.button.y - _this_dist.y;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::LeftDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::LeftUp, 
        [this](const Caller& caller, const SDL_Event& event)
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _this_dist = caller.second;
                int x = event.button.x - _this_dist.x;
                int y = event.button.y - _this_dist.y;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::LeftUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::RightDown, 
        [this](const Caller& caller, const SDL_Event& event)
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _this_dist = caller.second;
                int x = event.button.x - _this_dist.x;
                int y = event.button.y - _this_dist.y;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::RightDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::RightUp, 
        [this](const Caller& caller, const SDL_Event& event)
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _this_dist = caller.second;
                int x = event.button.x - _this_dist.x;
                int y = event.button.y - _this_dist.y;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::RightUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::MiddleDown, 
        [this](const Caller& caller, const SDL_Event& event)
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _this_dist = caller.second;
                int x = event.button.x - _this_dist.x;
                int y = event.button.y - _this_dist.y;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::MiddleDown, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
    Element::add_event_handler(
        TSDL::events::MiddleUp, 
        [this](const Caller& caller, const SDL_Event& event)
        {
            if(_current_mouse_focus.has_value())
            {
                point_2d _this_dist = caller.second;
                int x = event.button.x - _this_dist.x;
                int y = event.button.y - _this_dist.y;
                Subelement _actual_focus = _current_mouse_focus.value();
                _actual_focus.first.get().dispatch_event(Caller(*this, {x - _actual_focus.second.first.x, y - _actual_focus.second.first.y}), TSDL::events::MiddleUp, event);
                _current_mouse_focus.reset();
            }
            return false;
        }
    );
}