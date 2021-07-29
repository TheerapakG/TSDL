#include "TSDL.hpp"
#ifndef TSDL_EXPOSE_PYBIND11
#ifdef TSDL_EXAMPLE_CPP
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <memory>
#include <stack>
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

TSDL::Font* font = nullptr;

std::unique_ptr<el::SceneCommon> current_scene;

class DirectoryScene: public el::InstantBindScene<el::Grid, DirectoryScene>
{
    private:
    std::filesystem::path _path;
    el::FilledRectangle _bg = el::FilledRectangle({SCREEN_WIDTH, SCREEN_HEIGHT});
    el::Grid _m_grid;
    el::GridWithScrollbar _grid;
    std::stack<el::DependentElement*> elements;
    int _curr_y = 0;

    public:
    DirectoryScene(el::WindowAdapter& window, const std::filesystem::path& path);

    private:
    template <typename T, typename... Args>
    void _add(T* element, Args&&... args)
    {
        _grid.grid().add_child(*element, std::forward<Args>(args)...);
        elements.push(element);
    }

    void _add_title()
    {
        TSDL::Texture pathtext = TSDL::PangoLayout()
            .text("Current path: " + _path.u8string())
            .font(*font)
            .rendered_texture(bounded_window().renderer(), {1.0, 1.0, 1.0, 1.0});

        el::effectelement<el::TextureElement>* pathtexteffect = new el::effectelement<el::TextureElement>(
            pathtext.size(),
            std::make_shared<TSDL::Texture>(std::move(pathtext))
        );

        _add(pathtexteffect, TSDL::point_2d{0, _curr_y});

        TSDL::effects::fade_in(*pathtexteffect, 5000ms);

        _curr_y += pathtext.size().y + 16;
    }

    void _add_button(const std::_TSDL_U8(string)& str, el::EventHandler& on_press)
    {
        TSDL::Texture buttontext = TSDL::PangoLayout()
            .text(str)
            .font(*font)
            .rendered_texture(bounded_window().renderer(), {1.0, 1.0, 1.0, 1.0});

        TSDL::elements::TextureElement* buttontextelement = new TSDL::elements::TextureElement(
            buttontext.size(),
            std::make_shared<TSDL::Texture>(std::move(buttontext))
        );
        elements.push(buttontextelement);

        TSDL::elements::Button* button = new TSDL::elements::Button(
            TSDL::point_2d{buttontext.size().x + 32, 64}
        );
        button->front(*buttontextelement);
        button->on_button_activated() = on_press;

        _add(button, TSDL::point_2d{0, _curr_y});
        _curr_y += button->size().y;
    }

    void _add_parent_dir()
    {
        _add_button(
            "..",
            el::EventHandler{
                [this](const TSDL::elements::Caller& caller, const SDL_Event&) -> bool
                {
                    el::current_eventloop_adapter().register_call_next(
                        [this]()
                        {
                            current_scene = std::make_unique<DirectoryScene>(bounded_window(), _path.parent_path());
                        }
                    );
                    return true;
                }
            }
        );
    }

    void _add_subdirs()
    {
        for(auto p: std::filesystem::directory_iterator(_path, std::filesystem::directory_options::skip_permission_denied))
        {
            if(!p.is_directory()) continue;
            auto path = p.path();
            if(!path.has_stem()) continue;
            _add_button(
                path.stem().u8string(),
                el::EventHandler{
                    [this, path](const TSDL::elements::Caller& caller, const SDL_Event&) -> bool
                    {
                        el::current_eventloop_adapter().register_call_next(
                            [this, path]()
                            {
                                current_scene = std::make_unique<DirectoryScene>(bounded_window(), path);
                            }
                        );
                        return true;
                    }
                }
            );
        }
    }

    public:
    virtual el::Grid& target() override
    {
        return _m_grid;
    }

    virtual const el::Grid& target() const override
    {
        return _m_grid;
    }

    virtual void on_unbound_window(el::WindowAdapter&) override
    {
        current_scene = nullptr;
    }

    virtual ~DirectoryScene()
    {
        while(!elements.empty())
        {
            // probably the easiest way to ensure the destruction order
            delete elements.top();
            elements.pop(); 
        }
    }
};

DirectoryScene::DirectoryScene(el::WindowAdapter& window, const std::filesystem::path& path):
    el::InstantBindScene<el::Grid, DirectoryScene>{window},
    _path(path),
    _grid({SCREEN_WIDTH, SCREEN_HEIGHT}, 16)
{
    _m_grid.add_child(_bg, {0, 0});
    _m_grid.add_child(_grid, {0, 0});

    _add_title();
    _add_parent_dir();
    _add_subdirs();
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

        TSDL::Eventloop eventloop;

        eventloop.limit_fps();
        eventloop.fps_target(60);
        eventloop.add_event_handler(SDL_QUIT, quit_handler);

        TSDL::Window window("TSDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        TSDL::elements::EventloopAdapter evAdapter;
        TSDL::elements::WindowAdapter winAdapter(window);

        TSDL::Font font("Sans-Serif Normal 40");
        ::font = &font;

        std::thread t(say_fps);

        ::current_scene = std::make_unique<DirectoryScene>(winAdapter, std::filesystem::current_path());

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