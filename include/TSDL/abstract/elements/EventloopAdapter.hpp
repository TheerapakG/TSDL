//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

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

#include "TSDL/Meta.hpp"
#include "TSDL/Eventloop.hpp"
#include "TSDL/Renderer.hpp"

namespace TSDL
{
    namespace elements
    {
        class WindowAdapter;

        class EventloopAdapter
        {
            private:
            Eventloop& _evloop;
            std::set <::TSDL::EventHandler*> _handlers;
            std::unordered_map <Uint32, std::reference_wrapper<WindowAdapter>> _windows;

            std::queue<std::reference_wrapper<DependentElement>> _not_update_el;
            std::queue<std::function<void()>> _calls;

            void add_window(WindowAdapter& window_adapter);
            void remove_window(WindowAdapter& window_adapter);
            
            public:
            EventloopAdapter();

            ~EventloopAdapter();

            friend class WindowAdapter;

            /*
            Call not_update on the specified element after render finished
            */
            void register_not_update(DependentElement& element);

            /*
            Call `call` on next loop iteration
            */
            void register_call_next(std::function<void()> call);

            Eventloop::clock::time_point now();

            const std::unordered_map <Uint32, std::reference_wrapper<WindowAdapter>>& windows();
        };

        EventloopAdapter& current_eventloop_adapter();
    }
}

#endif