//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_WINDOWADAPTER_
#define TSDL_ELEMENTS_WINDOWADAPTER_

#include <vector>
#include <tuple>
#include <map>
#include <queue>
#include <functional>
#include <optional>
#include "TSDL/visuals/elements/attrs/EventLookup.hpp"

#include "TSDL/Meta.hpp"
#include "TSDL/Eventloop.hpp"
#include "TSDL/Renderer.hpp"

namespace TSDL
{
    extern SDL_Event null_event;

    namespace elements
    {
        class WindowAdapter: public attrs::staticeventlookup<Element, WindowAdapter>
        {
            private:
            optional_reference<attrs::EventLookupable> _src;
            optional_reference<DependentElement> _d_src;
            Window& _window;
            Renderer _renderer;
            
            public:
            WindowAdapter() = delete;
            WindowAdapter(Window& window);
            WindowAdapter(Window& window, Uint32 renderer_flags);

            ~WindowAdapter();

            Window& window();
            Renderer& renderer();

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
                if(_src) get_ref(_src).dispatch_event({*this, point_2d{0, 0}}, events::EventType::UnboundWindow, null_event);
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

            template <events::EventType eventtype>
            bool dispatch_templated_event(point_2d pos, const SDL_Event& event)
            {
                if(_src) return src<attrs::EventLookupable>().dispatch_event(Caller{*this, pos}, eventtype, event);
                return false;
            }

            bool dispatch_event(point_2d pos, events::EventType eventtype, const SDL_Event& event);

            template <events::EventType eventtype>
            bool dispatch_templated_event(const Caller& caller, const SDL_Event& event)
            {
                return dispatch_templated_event<eventtype>(caller.event_location, event);
            }

            void render();
        };
    }
}

#endif