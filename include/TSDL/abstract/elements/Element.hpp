#ifndef TSDL_ELEMENTS_ELEMENT_
#define TSDL_ELEMENTS_ELEMENT_

#include <atomic>
#include <vector>
#include <map>
#include <functional>
#include "TSDL/TSDL_Utility.hpp"
#include "TSDL/TSDL_Texture.hpp"
#include "TSDL/TSDL_Renderer.hpp"
#include "TSDL/abstract/events/EventType.hpp"

namespace TSDL
{
    namespace elements
    {
        class Element;

        class EventloopAdapter;
        class ElementHolder;

        namespace attrs
        {
            class EventDispatcher;
        }

        using Caller = std::pair<std::reference_wrapper<::TSDL::elements::attrs::EventDispatcher>, point_2d>;
        using EventHandler = std::function<bool(const Caller&, const SDL_Event&)>;

        /*
        All types inherited from Element shall inherit Element virtually
        */        
        class Element
        {
            // TODO: after_render queue for setting element as re-rendered callback and also other callback
            private:
            std::map <::TSDL::events::EventType, std::vector<EventHandler>> _evhdlrmap;
            
            public:
            Element() = default;
            Element(const Element&) = delete;

            virtual bool dispatch_event(const Caller& caller, const ::TSDL::events::EventType& eventtype, const SDL_Event& event);            
            void add_event_handler(const ::TSDL::events::EventType& eventtype, const EventHandler& evhandler);
            void remove_event_handler(const ::TSDL::events::EventType& eventtype);
        };

        bool operator==(const Element& lhs, const Element& rhs);
        bool operator!=(const Element& lhs, const Element& rhs);

        class DependentElement: virtual public Element
        {
            private:
            TSDL_Renderer& _renderer;
            EventloopAdapter& _evloop;
            std::atomic<bool> _update = true;
            std::vector<std::reference_wrapper<::TSDL::elements::ElementHolder>> _holders;

            public:
            DependentElement(EventloopAdapter& evloop, TSDL_Renderer& renderer);

            friend class EventloopAdapter;
            friend class ElementHolder;

            /*
            Get bounded holder
            */
            std::vector<std::reference_wrapper<::TSDL::elements::ElementHolder>> holder() const;

            /*
            Get bounded renderer
            */
            TSDL_Renderer& renderer() const;

            /*
            Get bounded eventloop
            */
            EventloopAdapter& eventloop() const;

            /*
            Make parent update this element on the next cycle
            */
            void update();

            /*
            Reverse what update() did after this cycle finished but before the next cycle begins
            */
            void not_update();

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const;

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) = 0;
        };

        class RenderSizedElement: public DependentElement
        {
            public:
            using DependentElement::DependentElement;

            /*
            Forces this element to be rendered with specified size
            */
            virtual void render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size) = 0;
        };
    }
}

#endif