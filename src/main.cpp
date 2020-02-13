#include "TSDL.hpp"
#ifndef TSDL_EXPOSE_PYBIND11
#ifdef TSDL_EXAMPLE_CPP
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <typeinfo>
using namespace std::literals::chrono_literals;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

TSDL::TSDL_Eventloop eventloop;

std::atomic_bool quit(false);

void quit_handler(const SDL_Event& event)
{
    eventloop.interrupt();
    quit = true;
}

void say_fps()
{    
    while (!quit.load())
    {
        std::cout << eventloop.fps() << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

int main(int argc, char* argv[])
{
    std::iostream::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cout << "starting..." << std::endl;

    try
    {
        //Initialize SDL
        TSDL::TSDL tsdl;

        TSDL::TSDL_Window window("TSDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
        TSDL::TSDL_Renderer renderer(window, SDL_RENDERER_TARGETTEXTURE);
        TSDL::elements::Grid grid(renderer, {SCREEN_WIDTH, SCREEN_HEIGHT});
        TSDL::elements::Button button(renderer, {256, 64});
        grid.add_child(button, {64, 64});
        TSDL::elements::EventloopAdapter elementAdapter(renderer, eventloop, grid);

        eventloop.add_event_handler(SDL_QUIT, quit_handler);

        std::thread t(say_fps);
        eventloop.run();
        t.join();        
    }
    catch(const std::exception& e)
    {
        std::cerr << "Caught exception: " << typeid(e).name() << "\n";
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;    
}
#endif
#endif