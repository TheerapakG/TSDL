#ifndef TSDL_ELEMENTS_BUTTON_
#define TSDL_ELEMENTS_BUTTON_

#include "TSDL/abstract/elements/Sized.hpp"
#include "TSDL/abstract/elements/EventDispatcher.hpp"

#include <string>

namespace TSDL
{
    namespace elements
    {
        enum class ButtonState
        {
            NORMAL,
            HOVER,
            CLICKED
        };

        class Button: public sized<eventdispatcher<Element>>
        {
            private:
            color_rgba _color = {211, 211, 211, 128};
            color_rgba _hover_color = {192, 192, 192, 128};
            color_rgba _clicked_color = {128, 128, 128, 255};
            color_rgba _text_color = {255, 255, 255, 255};
            std::string _text = "Button";

            ButtonState state = ButtonState::NORMAL;

            public:
            Button(TSDL_Renderer& renderer, const point_2d& size);
            Button(TSDL_Renderer& renderer, const ListenerMap& listeners, const point_2d& size);
            Button(TSDL_Renderer& renderer, ListenerMap&& listeners, const point_2d& size);

            Button& color(const color_rgba& color);
            Button& hover_color(const color_rgba& color);
            Button& clicked_color(const color_rgba& color);
            Button& text_color(const color_rgba& color);
            Button& text(const std::string& text);

            color_rgba color();
            color_rgba hover_color();
            color_rgba clicked_color();
            color_rgba text_color();
            std::string text();

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;
        };
    }
}

#endif