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

void TSDL::TSDL_Eventloop::_reset_fps_count()
{
    std::scoped_lock lock(_lock_frame_calc);
    _frame_since_last = 0;
    _time_since_last = clock::now();
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
            if(_track_fps)
            {
                clock::time_point now = clock::now();
                std::scoped_lock lock(_lock_frame_calc);
                clock::duration diff = now - _time_since_last;
                if(diff >= _fps_update_interval)
                {
                    _previous_fps.store(static_cast<double>(_frame_since_last + 1) / std::chrono::duration_cast<std::chrono::seconds>(diff).count());
                    this->_reset_fps_count();
                }
                else _frame_since_last ++;
            }
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
    if(_track_fps) this->_reset_fps_count();
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

void TSDL::TSDL_Eventloop::track_fps(bool track)
{
    this->_reset_fps_count();
    _track_fps = track;
}

void TSDL::TSDL_Eventloop::set_fps_update_interval(TSDL::TSDL_Eventloop::clock::duration interval)
{
    this->_reset_fps_count();
    _fps_update_interval = interval;
}

TSDL::TSDL_Eventloop::clock::duration TSDL::TSDL_Eventloop::get_fps_update_interval() const
{
    return _fps_update_interval;
}

double TSDL::TSDL_Eventloop::get_fps() const
{
    return _previous_fps.load();
}