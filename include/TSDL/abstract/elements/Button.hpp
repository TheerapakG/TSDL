//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_BUTTON_
#define TSDL_ELEMENTS_BUTTON_

#include "TSDL/abstract/elements/TextureElement.hpp"
#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/abstract/elements/attrs/NoneAttr.hpp"
#include "TSDL/abstract/elements/AttredType.hpp"
#include "TSDL/abstract/elements/FilledRectangle.hpp"
#include "TSDL/abstract/elements/WindowAdapter.hpp"

#include "TSDL/TSDL_Meta.hpp"
#include "TSDL/utilities/meta_list.hpp"
#include <memory>
#include <functional>

namespace TSDL::elements
{
    enum class ButtonState
    {
        NORMAL,
        HOVER,
        CLICKED
    };

    namespace impl
    {

        template <typename Derived>
        using _Button_Attrs = attrs::sizable<attrs::eventdispatcher<attrs::staticeventlookup<DependentElement, Derived>>>;

        template <typename Derived>
        class _Button: public _Button_Attrs<Derived>
        {
            public:
            using Base_T = typename _Button_Attrs<Derived>::Base_T;
            using Derived_T = typename _Button_Attrs<Derived>::Derived_T;

            private:
            std::shared_ptr<RenderSizedElement> _normal = std::make_shared<FilledRectangle>(point_2d{0, 0}, color_rgba{128, 128, 128, 128});
            std::shared_ptr<RenderSizedElement> _hover = std::make_shared<FilledRectangle>(point_2d{0, 0}, color_rgba{192, 192, 192, 128});
            std::shared_ptr<RenderSizedElement> _clicked = std::make_shared<FilledRectangle>(point_2d{0, 0}, color_rgba{211, 211, 211, 128});
            optional_reference<attrs::sizable<RenderSizedElement>> _front;
            int _padding = 8;

            ButtonState state = ButtonState::NORMAL;

            EventHandler _on_button_activated = always_true_event_handler;

            public:
            explicit _Button(const point_2d& size):
                _Button_Attrs<Derived>(size) {}
            explicit _Button(const point_2d& size, const attrs::ListenerMap& listeners):
                _Button_Attrs<Derived>(size, listeners) {}
            explicit _Button(const point_2d& size, attrs::ListenerMap&& listeners):
                _Button_Attrs<Derived>(size, listeners) {}

            EventHandler& on_button_activated()
            {
                return _on_button_activated;
            }

            template <events::EventType eventtype>
            bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
            {
                return _Button_Attrs<Derived>::dispatch_templated_event<eventtype>(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::ButtonActivated>(const Caller& caller, const SDL_Event& event)
            {
                return _on_button_activated(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::MouseIn>(const Caller&, const SDL_Event& event)
            {
                if(state != ButtonState::CLICKED)
                {
                    state = ButtonState::HOVER;
                    update(); 
                }
                return true;
            }

            template <>
            bool dispatch_templated_event<events::EventType::MouseOut>(const Caller&, const SDL_Event&)
            {
                if(state != ButtonState::CLICKED)
                {
                    state = ButtonState::NORMAL;
                    update();  
                }
                return true;
            }

            template <>
            bool dispatch_templated_event<events::EventType::LeftDown>(const Caller&, const SDL_Event&)
            {
                state = ButtonState::CLICKED;
                update();
                return true;
            }

            template <>
            bool dispatch_templated_event<events::EventType::LeftUp_Inside>(const Caller& caller, const SDL_Event&)
            {
                state = ButtonState::HOVER;
                dispatch_event(Caller{*this, {0, 0}}, events::EventType::ButtonActivated, ::TSDL::null_event);
                update();
                return true;
            }

            template <>
            bool dispatch_templated_event<events::EventType::LeftUp_Outside>(const Caller&, const SDL_Event&)
            {
                state = ButtonState::NORMAL;
                update();
                return true;
            }

            _Button<Derived>& normal(const std::shared_ptr<RenderSizedElement>& element)
            {
                _normal = element;
                return *this;
            }
            _Button<Derived>& hover(const std::shared_ptr<RenderSizedElement>& element)
            {
                _hover = element;
                return *this;
            }
            _Button<Derived>& clicked(const std::shared_ptr<RenderSizedElement>& element)
            {
                _clicked = element;
                return *this;
            }
            _Button<Derived>& front(optional_reference<attrs::sizable<RenderSizedElement>> front)
            {
                _front = front;
                return *this;
            }

            std::shared_ptr<RenderSizedElement> normal()
            {
                return _normal;
            }
            std::shared_ptr<RenderSizedElement> hover()
            {
                return _hover;
            }
            std::shared_ptr<RenderSizedElement> clicked()
            {
                return _clicked;
            }
            attrs::sizable<RenderSizedElement>& front()
            {
                return _front.value();
            }

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const override
            {
                switch (state)
                {
                case ButtonState::NORMAL:
                    if(_normal->need_update()) return true;
                    break;

                case ButtonState::HOVER:
                    if(_hover->need_update()) return true;
                    break;

                case ButtonState::CLICKED:
                    if(_clicked->need_update()) return true;
                    break;
                
                default:
                    break;
                }
                return _Button_Attrs<Derived>::need_update();
            }

            /*
            Re-render this element
            */
            virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) override
            {
                switch (state)
                {
                case ButtonState::NORMAL:
                    _normal->render(window, dist, size());
                    break;

                case ButtonState::HOVER:
                    _hover->render(window, dist, size());
                    break;

                case ButtonState::CLICKED:
                    _clicked->render(window, dist, size());
                    break;
                
                default:
                    break;
                }

                if(_front)
                {
                    attrs::sizable<RenderSizedElement>& _telement = _front.value().get();
                    const ::TSDL::point_2d texture_final_size = size() - 2 * ::TSDL::point_2d(_padding, _padding);
                    const ::TSDL::point_2d texture_size = _telement.size();
                    double x_ratio = static_cast<double>(texture_final_size.x) / texture_size.x;
                    double y_ratio = static_cast<double>(texture_final_size.y) / texture_size.y;
                    double ratio = std::min(x_ratio, y_ratio);
                    const ::TSDL::point_2d render_size(std::round(ratio * texture_size.x), std::round(ratio * texture_size.y));
                    if(ratio>0)
                    {
                        _telement.render(window, dist + (size() - render_size)/2, render_size);
                    }
                }

                _Button_Attrs<Derived>::not_update();
            }
        };

    }

    template <template <typename> typename... Attrs>
    class Button_: public ATTRedType<impl::_Button, Button_, Attrs...>
    {        
        template <typename ...Args>
        Button_(Args... args): ATTRedType<impl::_Button, Button_, Attrs...>(args...) {}

        Button_& normal(const std::shared_ptr<RenderSizedElement>& element)
        {
            Impl::normal(element);
            return *this;
        }

        Button_& hover(const std::shared_ptr<RenderSizedElement>& element)
        {
            Impl::hover(element);
            return *this;
        }

        Button_& clicked(const std::shared_ptr<RenderSizedElement>& element)
        {
            Impl::clicked(element);
            return *this;
        }

        Button_& front(optional_reference<attrs::sizable<RenderSizedElement>> front)
        {
            Impl::front(front);
            return *this;
        }
    };

    using Button = typename Button_<>;
}

#endif