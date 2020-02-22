#include "TSDL.hpp"
#ifndef TSDL_EXPOSE_PYBIND11
#ifdef TSDL_EXAMPLE_CPP
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <typeinfo>
#include <filesystem>
using namespace std::literals::chrono_literals;
namespace elattrs = TSDL::elements::attrs;

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
        TSDL::elements::Grid grid(renderer);
        elattrs::dragable<TSDL::elements::Button> button(
            renderer, 
            [](const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist) -> ::TSDL::point_2d { return start + dist; }, 
            ::TSDL::point_2d{256, 64}
        );

        #ifdef TSDL_USE_FONTCONFIG
        std::string font_path = TSDL::get_family_font_filename("sans-serif");
        std::cout << "using font: " << font_path << " as sans-serif font" << std::endl;
        TSDL::TSDL_Font font(font_path, 40);
        #else
        TSDL::TSDL_Font font((std::filesystem::current_path()/"fonts/segoeui.ttf").string(), 40);
        #endif

        TSDL::TSDL_Surface* buttontext = new TSDL::TSDL_Surface(u8"Drag Me!", font, {0xFF, 0xFF, 0xFF}, TSDL::TTF_Rendermethod::Blended);

        TSDL::elements::TextureElement buttontextelement(
            renderer, 
            buttontext->size(),
            std::shared_ptr <TSDL::TSDL_Texture> ( 
                new TSDL::TSDL_Texture(
                    renderer,
                    *buttontext
                )
            )
        );

        delete buttontext;

        button.texture(buttontextelement);
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