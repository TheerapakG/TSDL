#ifndef TSDL_ELEMENTS_EVENTLOOPADAPTER_
#define TSDL_ELEMENTS_EVENTLOOPADAPTER_

#include <vector>
#include <tuple>
#include <map>
#include <queue>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/EventDispatcher.hpp"

#include "TSDL/TSDL_Meta.hpp"
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
            optional_reference<DependentElement> _src;
            std::set <::TSDL::EventHandler*> _handlers;
            std::queue<std::reference_wrapper<DependentElement>> _not_update_el;
            
            public:
            EventloopAdapter() = delete;
            EventloopAdapter(TSDL_Renderer& renderer, TSDL_Eventloop& evloop);

            ~EventloopAdapter();

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;

            /*
            Call not_update on the specified element after render finished
            */
            void register_not_update(DependentElement& element);

            /*
            set source
            */
            void src(DependentElement& src);
            // set source to none
            void src(std::nullopt_t);

            DependentElement& src() const;
        };
    }
}

#endif