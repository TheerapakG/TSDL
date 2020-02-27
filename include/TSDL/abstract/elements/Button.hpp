#ifndef TSDL_ELEMENTS_BUTTON_
#define TSDL_ELEMENTS_BUTTON_

#include "TSDL/abstract/elements/TextureElement.hpp"
#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/abstract/elements/Rectangle.hpp"

#include "TSDL/TSDL_Meta.hpp"
#include <memory>

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

        class Button: public attrs::sizable<attrs::eventdispatcher<DependentElement>>
        {
            private:
            std::shared_ptr<RenderSizedElement> _normal = std::make_shared<Rectangle>(eventloop(), renderer(), point_2d{0, 0}, color_rgba{211, 211, 211, 128});
            std::shared_ptr<RenderSizedElement> _hover = std::make_shared<Rectangle>(eventloop(), renderer(), point_2d{0, 0}, color_rgba{192, 192, 192, 128});
            std::shared_ptr<RenderSizedElement> _clicked = std::make_shared<Rectangle>(eventloop(), renderer(), point_2d{0, 0}, color_rgba{128, 128, 128, 128});
            optional_reference<attrs::sizable<RenderSizedElement>> _front;
            int _padding = 8;

            ButtonState state = ButtonState::NORMAL;

            public:
            Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size);
            Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, const attrs::ListenerMap& listeners);
            Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, attrs::ListenerMap&& listeners);

            Button& normal(const std::shared_ptr<RenderSizedElement>& element);
            Button& hover(const std::shared_ptr<RenderSizedElement>& element);
            Button& clicked(const std::shared_ptr<RenderSizedElement>& element);
            Button& front(optional_reference<attrs::sizable<RenderSizedElement>> front);

            std::shared_ptr<RenderSizedElement> normal();
            std::shared_ptr<RenderSizedElement> hover();
            std::shared_ptr<RenderSizedElement> clicked();
            attrs::sizable<RenderSizedElement>& front();

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const override;

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;
        };
    }
}

#endif