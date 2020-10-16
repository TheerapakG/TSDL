//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/elements/WindowAdapter.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::elements
{
    WindowAdapter::WindowAdapter(Window& window): 
        _window(window), 
        _renderer(window)
    {
        current_eventloop_adapter().add_window(*this);
    }

    WindowAdapter::WindowAdapter(Window& window, Uint32 renderer_flags): 
        _window(window), 
        _renderer(window, renderer_flags)
    {
        current_eventloop_adapter().add_window(*this);
    }

    WindowAdapter::~WindowAdapter()
    {
        current_eventloop_adapter().remove_window(*this);
    }
            
    Window& WindowAdapter::window()
    {
        return _window;
    }
            
    Renderer& WindowAdapter::renderer()
    {
        return _renderer;
    }

    void WindowAdapter::src(std::nullopt_t)
    {
        _src.reset();
        _d_src.reset();
    }

    bool WindowAdapter::dispatch_event(point_2d pos, events::EventType eventtype, const SDL_Event& event)
    {
        return attrs::staticeventlookup<Element, WindowAdapter>::dispatch_event(Caller{*this, pos}, eventtype, event);
    }

    void WindowAdapter::render()
    {
        if (_d_src.has_value())
        {
            DependentElement& d_src = src <DependentElement>();
            if (!d_src.need_update()) return;
            renderer().clear({255, 255, 255, 255});
            d_src.render(*this, {0, 0});
            renderer().update();
        }
    }
}