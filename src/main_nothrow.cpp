#include "TSDL.hpp"
#ifndef TSDL_EXPOSE_PYBIND11
#ifdef TSDL_EXAMPLE_CPP
#ifndef __cpp_exceptions
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <typeinfo>
using namespace std::literals::chrono_literals;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

TSDL::TSDL_Window* window;
TSDL::TSDL_Eventloop eventloop;

void print_exc()
{
    std::exception& e = TSDL::get_exc();
    std::cerr << "Error: " << typeid(e).name() << "\n";
    std::cerr << e.what() << '\n';
}

template <typename T>
bool check(T& obj)
{
    if (!TSDL::check_integrity(obj))
    {
        print_exc();
        return false;
    }
    return true;
}

void render_handler()
{
    window->update_window_surface();
}

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

// Error value, keep in mind that all functions throw if exception is enabled
// so you only need to take error value only if exception is disabled
int e = 0;

int main(int argc, char* argv[])
{
    std::iostream::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cout << "starting..." << std::endl;

    TSDL::TSDL tsdl;
    if(!check(tsdl)) return -1;

    window = new TSDL::TSDL_Window("TSDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!check(*window)) return -1;
        
    //Get window surface
    auto screenSurface = window->window_surface_object();

    //Fill the surface white
    e = screenSurface.fill(0xFF, 0xFF, 0xFF);
    if (e != 0)
    {
        print_exc();
        return -1;
    }

    eventloop.render_function(render_handler);
    eventloop.add_event_handler(SDL_QUIT, quit_handler);  

    std::thread t(say_fps);
    eventloop.run();
    t.join();

    delete window;

    return 0;    
}
#endif
#endif
#endif