#include "TSDL.hpp"
#ifndef TSDL_EXPOSE_PYBIND11
#ifdef TSDL_EXAMPLE_CPP
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <typeinfo>
#include <filesystem>
using namespace std::literals::chrono_literals;
namespace el = TSDL::elements;
namespace elattrs = TSDL::elements::attrs;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

std::atomic_bool quit(false);

void quit_handler(const SDL_Event& event)
{
    TSDL::current_eventloop().interrupt();
    quit = true;
}

void say_fps()
{    
    while (!quit.load())
    {
        std::cout << TSDL::current_eventloop().fps() << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

TSDL::elements::Scene* current_scene = nullptr;
TSDL::elements::Grid* grid = nullptr;
TSDL::TSDL_Font* font = nullptr;
static std::filesystem::path current_path = std::filesystem::current_path();

std::vector <TSDL::elements::DependentElement*> _previous_visual_elements;
std::vector <TSDL::elements::DependentElement*> _dependent_visual_elements;

void generate_visual_from_path()
{
    for(auto el: _previous_visual_elements)
    {
        grid->remove_child(*el);
        delete el;
    }
    for(auto el: _dependent_visual_elements) delete el;

    _previous_visual_elements.clear();
    _dependent_visual_elements.clear();

    TSDL::TSDL_Texture pathtext = TSDL::TSDL_PangoLayout()
        .text("Current path: " + current_path.u8string())
        .font(*font)
        .rendered_texture(current_scene->bounded_window().renderer(), {1.0, 1.0, 1.0, 1.0});

    el::effectelement<el::TextureElement>* pathtexteffect = new el::effectelement<el::TextureElement>(
        pathtext.size(),
        std::shared_ptr <TSDL::TSDL_Texture> ( 
            new TSDL::TSDL_Texture(std::move(pathtext))
        )
    );

    grid->add_child(*pathtexteffect, {0, 0});
    _previous_visual_elements.push_back(pathtexteffect);

    int y = pathtext.size().y + 16;

    TSDL::TSDL_Texture buttontext = TSDL::TSDL_PangoLayout()
        .text("..")
        .font(*font)
        .rendered_texture(current_scene->bounded_window().renderer(), {1.0, 1.0, 1.0, 1.0});

    TSDL::elements::Button* button = new TSDL::elements::Button(
        TSDL::point_2d{buttontext.size().x + 32, 64}
    );

    TSDL::elements::TextureElement* buttontextelement = new TSDL::elements::TextureElement(
        buttontext.size(),
        std::shared_ptr <TSDL::TSDL_Texture> ( 
            new TSDL::TSDL_Texture(std::move(buttontext))
        )
    );

    button->front(*buttontextelement);
    button->on_button_activated() =
    [](const TSDL::elements::Caller& caller, const SDL_Event&) -> bool
    {
        current_path = current_path.parent_path();
        TSDL::elements::current_eventloop_adapter().register_call_next(generate_visual_from_path);
        return true;
    };

    grid->add_child(*button, {0, y});
    _previous_visual_elements.push_back(button);
    _dependent_visual_elements.push_back(buttontextelement);

    y += button->size().y;

    for(auto p: std::filesystem::directory_iterator(current_path, std::filesystem::directory_options::skip_permission_denied))
    {
        if(!p.is_directory()) continue;
        auto path = p.path();
        if(!path.has_stem()) continue;
        TSDL::TSDL_Texture buttontext = TSDL::TSDL_PangoLayout()
            .text(path.stem().u8string())
            .font(*font)
            .rendered_texture(current_scene->bounded_window().renderer(), {1.0, 1.0, 1.0, 1.0});

        TSDL::elements::Button* button = new TSDL::elements::Button(
            TSDL::point_2d{buttontext.size().x + 32, 64}
        );

        TSDL::elements::TextureElement* buttontextelement = new TSDL::elements::TextureElement(
            buttontext.size(),
            std::shared_ptr <TSDL::TSDL_Texture> ( 
                new TSDL::TSDL_Texture(std::move(buttontext))
            )
        );

        button->front(*buttontextelement);
        button->on_button_activated() =
        [path](const TSDL::elements::Caller& caller, const SDL_Event&) -> bool
        {
            current_path = path;
            TSDL::elements::current_eventloop_adapter().register_call_next(generate_visual_from_path);
            return true;
        };

        grid->add_child(*button, {0, y});
        _previous_visual_elements.push_back(button);
        _dependent_visual_elements.push_back(buttontextelement);

        y += button->size().y;

        TSDL::effects::fade_out(*pathtexteffect, 5000ms);
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

        TSDL::TSDL_Eventloop eventloop;

        eventloop.add_event_handler(SDL_QUIT, quit_handler);

        TSDL::TSDL_Window window("TSDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        TSDL::elements::EventloopAdapter evAdapter;
        TSDL::elements::WindowAdapter winAdapter(window);

        TSDL::elements::Grid bggrid;
        TSDL::elements::Scene _scene(winAdapter, bggrid);
        current_scene = &_scene;

        TSDL::elements::FilledRectangle bg({SCREEN_WIDTH, SCREEN_HEIGHT});
        bggrid.add_child(bg, {0, 0});

        TSDL::elements::GridWithScrollbar mgrid({SCREEN_WIDTH, SCREEN_HEIGHT}, 16);
        bggrid.add_child(mgrid, {0, 0});
        grid = &mgrid.grid();

        TSDL::TSDL_Font font("Sans-Serif Normal 40");
        ::font = &font;

        TSDL::elements::Button::push_back_attr<elattrs::dragable> button(
            [](const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist) -> ::TSDL::point_2d { return start + dist; }, 
            ::TSDL::point_2d{256, 64}
        );

        TSDL::TSDL_Texture buttontext = TSDL::TSDL_PangoLayout()
            .text(u8"Drag Me!")
            .font(font)
            .rendered_texture(current_scene->bounded_window().renderer(), {1.0, 1.0, 1.0, 1.0});

        TSDL::elements::TextureElement buttontextelement(
            buttontext.size(),
            std::shared_ptr <TSDL::TSDL_Texture> ( 
                new TSDL::TSDL_Texture(std::move(buttontext))
            )
        );

        button.front(buttontextelement);

        grid->add_child(button, {SCREEN_WIDTH-384, 0});

        generate_visual_from_path();

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