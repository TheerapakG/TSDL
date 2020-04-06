#ifndef TSDL_ELEMENTS_EVENTLOOPADAPTER_
#define TSDL_ELEMENTS_EVENTLOOPADAPTER_

#include <vector>
#include <tuple>
#include <map>
#include <queue>
#include <functional>
#include <optional>
#include "TSDL/abstract/elements/attrs/EventLookup.hpp"
#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"

#include "TSDL/TSDL_Meta.hpp"
#include "TSDL/TSDL_Eventloop.hpp"
#include "TSDL/TSDL_Renderer.hpp"

namespace TSDL
{
    namespace elements
    {
        class EventloopAdapter: public attrs::eventdispatcher<Element>
        {
            private:
            TSDL_Eventloop& _evloop;
            optional_reference<attrs::EventLookupable> _src;
            optional_reference<DependentElement> _d_src;
            std::set <::TSDL::EventHandler*> _handlers;
            std::queue<std::reference_wrapper<DependentElement>> _not_update_el;
            std::queue<std::function<void()>> _calls;
            
            public:
            EventloopAdapter() = delete;
            EventloopAdapter(TSDL_Eventloop& evloop);

            ~EventloopAdapter();

            /*
            Call not_update on the specified element after render finished
            */
            void register_not_update(DependentElement& element);

            /*
            Call `call` on next loop iteration
            */
            void register_call_next(std::function<void()> call);

            /*
            set source
            */
            template <typename T, typename U = std::enable_if_t<
                _and_v<
                    std::is_base_of_v<DependentElement, T>,
                    std::is_base_of_v<attrs::EventLookupable, T>
                >
            >>
            void src(T& src)
            {
                _src = src;
                _d_src = src;
            }

            // set source to none
            void src(std::nullopt_t);

            template<typename T>
            T& src() const;

            template<>
            attrs::EventLookupable& src<attrs::EventLookupable>() const
            {
                return _src.value().get();
            }

            template<>
            DependentElement& src<DependentElement>() const
            {
                return _d_src.value().get();
            }
        };
    }
}

#endif