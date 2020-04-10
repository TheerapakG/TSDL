#ifndef TSDL_ELEMENTS_ATTRS_EVENTLOOKUP_
#define TSDL_ELEMENTS_ATTRS_EVENTLOOKUP_

#include "TSDL/abstract/elements/Element.hpp"
#include <type_traits>

#define STATIC_LOOKUP_CASE(eventtype)                                                                       \
case eventtype:                                                                                             \
    return static_cast<Derived*>(this)->template dispatch_templated_event<eventtype>(caller, event);        \

namespace TSDL::elements::attrs
{
    class EventLookupable
    {
        public:
        virtual bool dispatch_event(const Caller& caller, events::EventType eventtype, const SDL_Event& event) = 0;
    };

    template <typename T>
    class dynamiceventlookup: virtual public EventLookupable
    {
        public:
        using Base_T = T;
        
        private:
        std::map <::TSDL::events::EventType, std::vector<EventHandler>> _evhdlrmap;

        public:
        template <typename ...Args>
        dynamiceventlookup(Args... args): T(args...) {}

        void add_event_handler(::TSDL::events::EventType eventtype, const EventHandler& evhandler)
        {
            _evhdlrmap[eventtype].emplace_back(evhandler);
        }
        void remove_event_handler(::TSDL::events::EventType eventtype)
        {
            _evhdlrmap.erase(eventtype);
        }

        virtual bool dispatch_event(const Caller& caller, events::EventType eventtype, const SDL_Event& event) override
        {
            std::vector<EventHandler> h;

            try
            {
                h = _evhdlrmap.at(eventtype);
            }
            catch (const std::out_of_range&)
            {
                return false;
            }

            bool _ret = false;

            for (auto& hi : h) _ret |= hi(caller, event);

            return _ret;
        }
    };

    template <typename T, typename Derived>
    class staticeventlookup: public T, virtual public EventLookupable
    {
        public:
        using Base_T = T;
        using Derived_T = Derived;

        public:
        template <typename ...Args>
        staticeventlookup(Args... args): T(args...){}

        template<events::EventType eventtype>
        bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
        {
            return false;
        }

        virtual bool dispatch_event(const Caller& caller, events::EventType eventtype, const SDL_Event& event)
        {
            // constexpr arg when?
            switch (eventtype)
            {
                STATIC_LOOKUP_CASE(events::EventType::WindowShown)
                STATIC_LOOKUP_CASE(events::EventType::WindowHidden)
                STATIC_LOOKUP_CASE(events::EventType::WindowExposed)
                STATIC_LOOKUP_CASE(events::EventType::WindowMoved)
                STATIC_LOOKUP_CASE(events::EventType::WindowResized)
                STATIC_LOOKUP_CASE(events::EventType::WindowMinimized)
                STATIC_LOOKUP_CASE(events::EventType::WindowMaximized)
                STATIC_LOOKUP_CASE(events::EventType::WindowRestored)
                STATIC_LOOKUP_CASE(events::EventType::WindowClose)
                STATIC_LOOKUP_CASE(events::EventType::KeyDown)
                STATIC_LOOKUP_CASE(events::EventType::KeyUp)
                STATIC_LOOKUP_CASE(events::EventType::TextInput)
                STATIC_LOOKUP_CASE(events::EventType::MouseMotion)
                STATIC_LOOKUP_CASE(events::EventType::MouseIn)
                STATIC_LOOKUP_CASE(events::EventType::MouseOut)
                STATIC_LOOKUP_CASE(events::EventType::LeftDown)
                STATIC_LOOKUP_CASE(events::EventType::LeftUp)
                STATIC_LOOKUP_CASE(events::EventType::LeftUp_Inside) // LeftDown originated in this element and LeftUp originated in this element
                STATIC_LOOKUP_CASE(events::EventType::LeftUp_Outside) // LeftDown originated in this element but LeftUp not originated in this element
                STATIC_LOOKUP_CASE(events::EventType::RightDown)
                STATIC_LOOKUP_CASE(events::EventType::RightUp)
                STATIC_LOOKUP_CASE(events::EventType::RightUp_Inside) // Like Left*
                STATIC_LOOKUP_CASE(events::EventType::RightUp_Outside) // Like Left*
                STATIC_LOOKUP_CASE(events::EventType::MiddleDown)
                STATIC_LOOKUP_CASE(events::EventType::MiddleUp)
                STATIC_LOOKUP_CASE(events::EventType::MiddleUp_Inside) // Like Left*
                STATIC_LOOKUP_CASE(events::EventType::MiddleUp_Outside) // Like Left*
                STATIC_LOOKUP_CASE(events::EventType::MouseWheel)
                STATIC_LOOKUP_CASE(events::EventType::ElementResized)
                STATIC_LOOKUP_CASE(events::EventType::ButtonActivated)
                STATIC_LOOKUP_CASE(events::EventType::Dragged) // Dragable dragged
                default:
                    break;
            }
            return false;
        }
    };
}

#endif