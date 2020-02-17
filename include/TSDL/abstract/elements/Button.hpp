#ifndef TSDL_ELEMENTS_BUTTON_
#define TSDL_ELEMENTS_BUTTON_

#include "TSDL/abstract/elements/TextureElement.hpp"
#include "TSDL/abstract/elements/EventDispatcher.hpp"

#include "TSDL/TSDL_Meta.hpp"

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
            optional_reference<TextureElement> _texture;
            int _padding = 8;

            ButtonState state = ButtonState::NORMAL;

            public:
            Button(TSDL_Renderer& renderer, const point_2d& size);
            Button(TSDL_Renderer& renderer, const point_2d& size, const ListenerMap& listeners);
            Button(TSDL_Renderer& renderer, const point_2d& size, ListenerMap&& listeners);

            Button& color(const color_rgba& color);
            Button& hover_color(const color_rgba& color);
            Button& clicked_color(const color_rgba& color);
            Button& text_color(const color_rgba& color);
            Button& texture(optional_reference<TextureElement> texture);

            color_rgba color();
            color_rgba hover_color();
            color_rgba clicked_color();
            color_rgba text_color();
            TextureElement& texture();

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;
        };
    }
}

#endif