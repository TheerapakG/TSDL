#ifndef TSDL_ELEMENTS_ELEMENT_
#define TSDL_ELEMENTS_ELEMENT_

#include <atomic>
#include <map>
#include <functional>
#include "TSDL_Utility.hpp"
#include "TSDL_Texture.hpp"
#include "TSDL_Renderer.hpp"
#include "abstract/events/EventType.hpp"

namespace TSDL
{
    namespace elements
    {
        class Element;

        using Caller = std::pair<std::reference_wrapper<Element>, point_2d>;
        using EventHandler = std::function<bool(const Caller&, const SDL_Event&)>;
        
        class Element
        {
            private:
            std::atomic<bool> _update = false;
            std::map <TSDL::events::EventType, EventHandler> _evhdlrmap;
            
            public:
            bool operator==(const Element& other);

            /*
            Make parent update this element on the next cycle
            */
            void update();

            /*
            Query if parent need to update this element on the next cycle
            */
            bool need_update();

            /*
            Re-render this element
            */
            virtual void render() = 0;

            /*
            Get previously rendered texture
            */
            virtual TSDL_Texture& get_texture() = 0;

            virtual bool dispatch_event(const Caller& caller, const TSDL::events::EventType& eventtype, const SDL_Event& event);            
            void add_event_handler(const TSDL::events::EventType& eventtype, const EventHandler& evhandler);
            void remove_event_handler(const TSDL::events::EventType& eventtype);
        };
    }
}

#endif