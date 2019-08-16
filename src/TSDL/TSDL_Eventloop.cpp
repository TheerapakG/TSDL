#include "TSDL/TSDL_Eventloop.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

TSDL::TSDL_Eventloop::TSDL_Eventloop() {}

TSDL::TSDL_Eventloop::TSDL_Eventloop(bool thrownoevhandler, bool thrownorenderhandler) :
_throw_if_no_event_handler(thrownoevhandler),
_throw_if_no_render_handler(thrownorenderhandler)
{}

TSDL::TSDL_Eventloop::~TSDL_Eventloop()
{
    if(_is_running)
    {
        this->interrupt();
    }
}

void TSDL::TSDL_Eventloop::add_event_handler(SDL_EventType evType, EventHandler handler)
{
    _map[evType] = handler;
}

void TSDL::TSDL_Eventloop::remove_event_handler(SDL_EventType evType)
{
    _map.erase(evType);
}

void TSDL::TSDL_Eventloop::set_render_function(RenderHandler handler)
{
    _render = handler;
}

void TSDL::TSDL_Eventloop::_run_step()
{
    SDL_Event e;
    if(SDL_PollEvent(&e) != 0)
    {
        EventHandler h;
        try
        {
            h = _map.at(static_cast<SDL_EventType>(e.type));
        }
        catch(const std::out_of_range& exc)
        {
            if(_throw_if_no_event_handler)
            {
                std::throw_with_nested(std::runtime_error("No handler for SDL event: " + std::to_string(e.type)));
            }
            else
            {
                std::cerr << "No handler for SDL event: " << e.type << " (" << exc.what() << "). You should consider adding handler." << std::endl;
                return;
            }
        }
        h(e);
        return;
    }
    else
    {
        if(_render != nullptr)
        {
            _render();
            return;
        }
        else
        {
            if(_throw_if_no_render_handler)
            {
                std::throw_with_nested(std::runtime_error("No handler for rendering"));
            }
            else
            {
                std::cerr << "No handler for rendering." << std::endl;
                return;
            }
        }
        
    }
}

void TSDL::TSDL_Eventloop::run()
{
    _is_running = true;
    while(_is_running)
    {
        this->_run_step();
    }
}

void TSDL::TSDL_Eventloop::interrupt()
{
    if(_is_running)
    {
        _is_running = false;
    }
    else
    {
        throw std::runtime_error("Could not interrupt while event loop is not already running!");
    }
    
}