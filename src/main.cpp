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

static TSDL::elements::Grid* grid = nullptr;
TSDL::elements::BaseHorizontalScrollbar* hscrollbar = nullptr;
TSDL::elements::BaseVerticalScrollbar* vscrollbar = nullptr;
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

    TSDL::TSDL_Surface pathtext("Current path: " + current_path.u8string(), *font, {0xFF, 0xFF, 0xFF}, TSDL::TTF_Rendermethod::Blended);

    TSDL::elements::TextureElement* pathtextelement = new TSDL::elements::TextureElement(
        grid->eventloop(), grid->renderer(),
        pathtext.size(),
        std::shared_ptr <TSDL::TSDL_Texture> ( 
            new TSDL::TSDL_Texture(
                grid->renderer(),
                pathtext
            )
        )
    );

    grid->add_child(*pathtextelement, {64, 64});
    _previous_visual_elements.push_back(pathtextelement);

    int y = 64 + pathtext.size().y + 16;

    TSDL::TSDL_Surface buttontext("..", *font, {0xFF, 0xFF, 0xFF}, TSDL::TTF_Rendermethod::Blended);
    TSDL::elements::TextureElement* buttontextelement = new TSDL::elements::TextureElement(
        grid->eventloop(), grid->renderer(),
        buttontext.size(),
        std::shared_ptr <TSDL::TSDL_Texture> ( 
            new TSDL::TSDL_Texture(
                grid->renderer(),
                buttontext
            )
        )
    );

    TSDL::elements::Button* button = new TSDL::elements::Button(
        grid->eventloop(), grid->renderer(),
        {buttontext.size().x + 32, 64}
    );

    button->front(*buttontextelement);
    button->add_event_handler(
        TSDL::events::EventType::ButtonActivated,
        [](const TSDL::elements::Caller&, const SDL_Event&) -> bool
        {
            current_path = current_path.parent_path();
            grid->eventloop().register_call_next(generate_visual_from_path);
            return true;
        }
    );

    grid->add_child(*button, {64, y});
    _previous_visual_elements.push_back(button);
    _dependent_visual_elements.push_back(buttontextelement);

    y += button->size().y;

    for(auto p: std::filesystem::directory_iterator(current_path, std::filesystem::directory_options::skip_permission_denied))
    {
        if(!p.is_directory()) continue;
        auto path = p.path();
        if(!path.has_stem()) continue;
        TSDL::TSDL_Surface buttontext(path.stem().u8string(), *font, {0xFF, 0xFF, 0xFF}, TSDL::TTF_Rendermethod::Blended);

        TSDL::elements::TextureElement* buttontextelement = new TSDL::elements::TextureElement(
            grid->eventloop(), grid->renderer(),
            buttontext.size(),
            std::shared_ptr <TSDL::TSDL_Texture> ( 
                new TSDL::TSDL_Texture(
                    grid->renderer(),
                    buttontext
                )
            )
        );

        TSDL::elements::Button* button = new TSDL::elements::Button(
            grid->eventloop(), grid->renderer(),
            {buttontext.size().x + 32, 64}
        );

        button->front(*buttontextelement);
        button->add_event_handler(
            TSDL::events::EventType::ButtonActivated,
            [path](const TSDL::elements::Caller&, const SDL_Event&) -> bool
            {
                current_path = path;
                grid->eventloop().register_call_next(generate_visual_from_path);
                return true;
            }
        );

        grid->add_child(*button, {64, y});
        _previous_visual_elements.push_back(button);
        _dependent_visual_elements.push_back(buttontextelement);

        y += button->size().y;
    }
    vscrollbar->content_height(std::max(2*SCREEN_HEIGHT, static_cast<TSDL::_rect>(grid->bound()).second.y + 16));    
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

        eventloop.add_event_handler(SDL_QUIT, quit_handler);

        TSDL::TSDL_Window window("TSDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
        TSDL::TSDL_Renderer renderer(window, SDL_RENDERER_TARGETTEXTURE);

        TSDL::elements::EventloopAdapter evAdapter(eventloop);

        TSDL::elements::Grid mgrid(evAdapter, renderer);
        evAdapter.src(mgrid);

        TSDL::elements::FilledRectangle bg(evAdapter, renderer, {SCREEN_WIDTH, SCREEN_HEIGHT});
        mgrid.add_child(bg, {0, 0});

        TSDL::elements::Grid grid(evAdapter, renderer);
        mgrid.add_child(grid, {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT});
        ::grid = &grid;

        #ifdef TSDL_USE_FONTCONFIG
        std::string font_path = TSDL::get_family_font_filename("sans-serif");
        std::cout << "using font: " << font_path << " as sans-serif font" << std::endl;
        TSDL::TSDL_Font font(font_path, 40);
        #else
        TSDL::TSDL_Font font((std::filesystem::current_path()/"fonts/segoeui.ttf").string(), 40);
        #endif
        ::font = &font;

        elattrs::dragable<TSDL::elements::Button> button(
            evAdapter, renderer,
            [](const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist) -> ::TSDL::point_2d { return start + dist; }, 
            ::TSDL::point_2d{256, 64}
        );

        TSDL::TSDL_Surface* buttontext = new TSDL::TSDL_Surface(u8"Drag Me!", font, {0xFF, 0xFF, 0xFF}, TSDL::TTF_Rendermethod::Blended);

        TSDL::elements::TextureElement buttontextelement(
            evAdapter, renderer,
            buttontext->size(),
            std::shared_ptr <TSDL::TSDL_Texture> ( 
                new TSDL::TSDL_Texture(
                    renderer,
                    *buttontext
                )
            )
        );

        delete buttontext;

        button.front(buttontextelement);
        grid.add_child(button, {SCREEN_WIDTH-320, 64});

        TSDL::elements::BaseHorizontalScrollbar hscrollbar(evAdapter, renderer, 2*SCREEN_WIDTH, {SCREEN_WIDTH-16, 16});
        hscrollbar.dispatch_event_direct(TSDL::events::EventType::Dragged, grid);
        ::hscrollbar = &hscrollbar;
        TSDL::elements::BaseVerticalScrollbar vscrollbar(evAdapter, renderer, 2*SCREEN_HEIGHT, {16, SCREEN_HEIGHT-16});
        vscrollbar.dispatch_event_direct(TSDL::events::EventType::Dragged, grid);
        ::vscrollbar = &vscrollbar;
        grid.add_event_handler(
            TSDL::events::EventType::Dragged, 
            [&grid, &hscrollbar, &vscrollbar](const TSDL::elements::Caller&, const SDL_Event&) -> bool
            {
                TSDL::point_2d region_x = hscrollbar.represented_section();
                TSDL::point_2d region_y = vscrollbar.represented_section();
                grid.render_position({region_x.x, region_y.x, region_x.y, region_y.y});
                return true;
            }
        );
        mgrid.add_child(hscrollbar, {0, SCREEN_HEIGHT-16});
        mgrid.add_child(vscrollbar, {SCREEN_WIDTH-16, 0});

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