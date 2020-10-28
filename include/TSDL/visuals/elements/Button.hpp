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

#include "TSDL/visuals/elements/TextureElement.hpp"
#include "TSDL/visuals/elements/attrs/EventDispatcher.hpp"
#include "TSDL/visuals/elements/attrs/NoneAttr.hpp"
#include "TSDL/visuals/elements/AttredType.hpp"
#include "TSDL/visuals/elements/FilledRectangle.hpp"
#include "TSDL/visuals/elements/WindowAdapter.hpp"

#include "TSDL/Meta.hpp"
#include "TSDL/utilities/optional_with_base.hpp"
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
        using _Button_Attrs = attrs::sized<attrs::eventdispatcher<attrs::staticeventlookup<DependentElement, Derived>>>;

        template <typename Derived>
        using _Sizable_Button_Attrs = attrs::sizable<attrs::eventdispatcher<attrs::staticeventlookup<DependentElement, Derived>>>;

        template <typename Derived>
        class _Button_Base : public Derived
        {
            public:
            using Button_Base_T = _Button_Base<Derived>;

            private:
            util::optional_with_base<attrs::Sized, DependentElement> _front;

            ButtonState state = ButtonState::NORMAL;
            EventHandler _on_button_activated = always_true_event_handler;

            public:
            template <typename... Args>
            _Button_Base(Args&&... args): Derived(std::forward<Args>(args)...) {}
            
            EventHandler& on_button_activated()
            {
                return _on_button_activated;
            }

            typename Derived::Derived_T& front(const util::optional_with_base<attrs::Sized, DependentElement>& front)
            {
                _front = front;
                return static_cast<Derived_T&>(*this);
            }

            const util::optional_with_base<attrs::Sized, DependentElement>& front() const
            {
                return _front;
            }

            template <events::EventType eventtype>
            bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
            {
                return Derived::dispatch_templated_event<eventtype>(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::ButtonActivated>(const Caller& caller, const SDL_Event& event)
            {
                return _on_button_activated(caller, event);
            }

            template <>
            bool dispatch_templated_event<events::EventType::MouseIn>(const Caller&, const SDL_Event& event)
            {
                if (state != ButtonState::CLICKED)
                {
                    state = ButtonState::HOVER;
                    update();
                }
                return true;
            }

            template <>
            bool dispatch_templated_event<events::EventType::MouseOut>(const Caller&, const SDL_Event&)
            {
                if (state != ButtonState::CLICKED)
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
                dispatch_event(Caller{ *this, {0, 0} }, events::EventType::ButtonActivated, ::TSDL::null_event);
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

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const override
            {
                switch (state)
                {
                case ButtonState::NORMAL:
                    if (static_cast<const Derived_T*>(this)->normal_p()->need_update()) return true;
                    break;

                case ButtonState::HOVER:
                    if (static_cast<const Derived_T*>(this)->hover_p()->need_update()) return true;
                    break;

                case ButtonState::CLICKED:
                    if (static_cast<const Derived_T*>(this)->clicked_p()->need_update()) return true;
                    break;

                default:
                    break;
                }

                if (_front)
                {
                    if (util::get<DependentElement>(_front).need_update()) return true;
                }
                return Derived::need_update();
            }

            /*
            Re-render this element
            */
            virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) const override
            {
                switch (state)
                {
                // TODO: if constexpr RenderSizedElement
                case ButtonState::NORMAL:
                    static_cast<const Derived_T*>(this)->normal_p()->render(window, dist, size());
                    break;

                case ButtonState::HOVER:
                    static_cast<const Derived_T*>(this)->hover_p()->render(window, dist, size());
                    break;

                case ButtonState::CLICKED:
                    static_cast<const Derived_T*>(this)->clicked_p()->render(window, dist, size());
                    break;

                default:
                    break;
                }

                if (_front)
                {
                    const ::TSDL::point_2d render_size = util::get<attrs::Sized>(_front).size();
                    util::get<DependentElement>(_front).render(window, dist + (size() - render_size) / 2);
                }

                Derived::not_update();
            }
        };

        template <typename Derived>
        class _Button: public _Button_Base<_Button_Attrs<Derived>>
        {
            private:
            point_2d _size;
            // TODO: less restrictive (sized element rather than render sized element)
            std::shared_ptr<RenderSizedElement> _normal = std::make_shared<FilledRectangle>(_size, color_rgba{128, 128, 128, 128});
            std::shared_ptr<RenderSizedElement> _hover = std::make_shared<FilledRectangle>(_size, color_rgba{192, 192, 192, 128});
            std::shared_ptr<RenderSizedElement> _clicked = std::make_shared<FilledRectangle>(_size, color_rgba{211, 211, 211, 128});

            public:
            explicit _Button(const point_2d& size):
                _Button_Base<_Button_Attrs<Derived>>(), _size(size) {}
            explicit _Button(const point_2d& size, const attrs::ListenerMap& listeners):
                _Button_Base<_Button_Attrs<Derived>>(listeners), _size(size) {}
            explicit _Button(const point_2d& size, attrs::ListenerMap&& listeners):
                _Button_Base<_Button_Attrs<Derived>>(listeners), _size(size) {}

            virtual point_2d size() const final
            {
                return _size;
            }

            // TODO: check size
            typename _Button::Button_Base_T::Derived_T& normal(const std::shared_ptr<RenderSizedElement>& element)
            {
                _normal = element;
                return static_cast<Derived_T&>(*this);
            }
            typename _Button::Button_Base_T::Derived_T& hover(const std::shared_ptr<RenderSizedElement>& element)
            {
                _hover = element;
                return static_cast<Derived_T&>(*this);
            }
            typename _Button::Button_Base_T::Derived_T& clicked(const std::shared_ptr<RenderSizedElement>& element)
            {
                _clicked = element;
                return static_cast<Derived_T&>(*this);
            }

            const std::shared_ptr<RenderSizedElement>& normal() const
            {
                return _normal;
            }
            const std::shared_ptr<RenderSizedElement>& hover() const
            {
                return _hover;
            }
            const std::shared_ptr<RenderSizedElement>& clicked() const
            {
                return _clicked;
            }

            const RenderSizedElement* normal_p() const
            {
                return _normal.get();
            }
            const RenderSizedElement* hover_p() const
            {
                return _hover.get();
            }
            const RenderSizedElement* clicked_p() const
            {
                return _clicked.get();
            }
        };

        template <typename Derived>
        class _Sizable_Button : public _Button_Base<_Sizable_Button_Attrs<Derived>>
        {
        private:
            std::shared_ptr<RenderSizedElement> _normal = std::make_shared<FilledRectangle>(point_2d{ 0, 0 }, color_rgba{ 128, 128, 128, 128 });
            std::shared_ptr<RenderSizedElement> _hover = std::make_shared<FilledRectangle>(point_2d{ 0, 0 }, color_rgba{ 192, 192, 192, 128 });
            std::shared_ptr<RenderSizedElement> _clicked = std::make_shared<FilledRectangle>(point_2d{ 0, 0 }, color_rgba{ 211, 211, 211, 128 });

        public:
            explicit _Sizable_Button(const point_2d& size) :
                _Button_Base<_Sizable_Button_Attrs<Derived>>(size) {}
            explicit _Sizable_Button(const point_2d& size, const attrs::ListenerMap& listeners) :
                _Button_Base<_Sizable_Button_Attrs<Derived>>(size, listeners) {}
            explicit _Sizable_Button(const point_2d& size, attrs::ListenerMap&& listeners) :
                _Button_Base<_Sizable_Button_Attrs<Derived>>(size, listeners) {}

            typename _Sizable_Button::Button_Base_T::Derived_T& normal(const std::shared_ptr<RenderSizedElement>& element)
            {
                _normal = element;
                return static_cast<Derived_T&>(*this);
            }
            typename _Sizable_Button::Button_Base_T::Derived_T& hover(const std::shared_ptr<RenderSizedElement>& element)
            {
                _hover = element;
                return static_cast<Derived_T&>(*this);
            }
            typename _Sizable_Button::Button_Base_T::Derived_T& clicked(const std::shared_ptr<RenderSizedElement>& element)
            {
                _clicked = element;
                return static_cast<Derived_T&>(*this);
            }

            const std::shared_ptr<RenderSizedElement>& normal() const
            {
                return _normal;
            }
            const std::shared_ptr<RenderSizedElement>& hover() const
            {
                return _hover;
            }
            const std::shared_ptr<RenderSizedElement>& clicked() const
            {
                return _clicked;
            }

            const RenderSizedElement* normal_p() const
            {
                return _normal.get();
            }
            const RenderSizedElement* hover_p() const
            {
                return _hover.get();
            }
            const RenderSizedElement* clicked_p() const
            {
                return _clicked.get();
            }
        };

    }

    template <template <typename> typename... Attrs>
    class Button_: public ATTRedType<impl::_Button, Button_, Attrs...> 
    {
        public:
        template <typename... Args>
        Button_(Args&&... args): ATTRedType<impl::_Button, Button_, Attrs...>(std::forward<Args>(args)...) {}

        using ATTRedType<impl::_Button, Button_, Attrs...>::normal_p;
        using ATTRedType<impl::_Button, Button_, Attrs...>::hover_p;
        using ATTRedType<impl::_Button, Button_, Attrs...>::clicked_p;
    };

    using Button = typename Button_<>;

    template <template <typename> typename... Attrs>
    class Sizable_Button_ : public ATTRedType<impl::_Sizable_Button, Sizable_Button_, Attrs...>
    {
        public:
        template <typename... Args>
        Sizable_Button_(Args&&... args) : ATTRedType<impl::_Sizable_Button, Sizable_Button_, Attrs...>(std::forward<Args>(args)...) {}

        using ATTRedType<impl::_Sizable_Button, Sizable_Button_, Attrs...>::normal_p;
        using ATTRedType<impl::_Sizable_Button, Sizable_Button_, Attrs...>::hover_p;
        using ATTRedType<impl::_Sizable_Button, Sizable_Button_, Attrs...>::clicked_p;
    };

    using Sizable_Button = typename Sizable_Button_<>;
}

#endif