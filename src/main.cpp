#include "TSDL.hpp"
#ifndef TSDL_EXPOSE_PYBIND11
#ifdef TSDL_EXAMPLE_CPP
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

TSDL::TSDL_Window* window;
TSDL::TSDL_Eventloop eventloop;

void render_handler()
{
    window->update_window_surface();
}

void quit_handler(SDL_Event event)
{
    eventloop.interrupt();
}

int main(int argc, char* args[])
{
    std::iostream::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Initialize SDL
    TSDL::TSDL tsdl;
    
    window = new TSDL::TSDL_Window("TSDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //Get window surface
    auto screenSurface = window->get_window_surface_object();

    //Fill the surface white
    screenSurface.fill_rect(NULL, 0xFF, 0xFF, 0xFF );

    eventloop.set_render_function(render_handler);
    eventloop.add_event_handler(SDL_QUIT, quit_handler);
    eventloop.run();

    delete window;

    return 0;
}
#endif
#endif