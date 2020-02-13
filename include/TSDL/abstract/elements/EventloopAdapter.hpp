#ifndef TSDL_ELEMENTS_EVENTLOOPADAPTER_
#define TSDL_ELEMENTS_EVENTLOOPADAPTER_

#include <vector>
#include <tuple>
#include <map>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/EventDispatcher.hpp"

#include "TSDL/TSDL_Eventloop.hpp"
#include "TSDL/TSDL_Renderer.hpp"

namespace TSDL
{
    namespace elements
    {
        class EventloopAdapter: public eventdispatcher<Element>
        {
            private:
            TSDL_Eventloop& _evloop;
            Element& _src;
            std::set <::TSDL::EventHandler*> _handlers;
            
            public:
            EventloopAdapter() = delete;
            EventloopAdapter(TSDL_Renderer& renderer, TSDL_Eventloop& evloop, Element& src);

            ~EventloopAdapter();

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;

            Element& src() const;
        };
    }
}

#endif