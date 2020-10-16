//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/effects/fade.hpp"
#include "TSDL/abstract/elements/EffectElement.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::effects
{
    void fade_in(elements::EffectElement& fx_element, const std::chrono::milliseconds& ms)
    {
        elements::EventloopAdapter& _ev_adapter = elements::current_eventloop_adapter();
        Eventloop::clock::time_point _start_time = _ev_adapter.now();
        Eventloop::clock::time_point _end_time = _start_time + ms;

        fx_element.modify_texture_function() = [=, &fx_element, &_ev_adapter](Texture& texture) -> void
        {
            if (_ev_adapter.now() >= _end_time)
            {
                if (texture.blend_mode() == SDL_BLENDMODE_BLEND) texture.blend_mode(SDL_BLENDMODE_NONE);
                fx_element.modify_texture_function() = nullptr;
                return;
            }
            else
            {
                if (texture.blend_mode() == SDL_BLENDMODE_NONE) texture.blend_mode(SDL_BLENDMODE_BLEND);
                std::chrono::duration<double, std::milli> _duration = _ev_adapter.now() - _start_time;
                double _opacity_multiplier = _duration / ms;
                texture.alpha_multiplier(static_cast<Uint8>(_opacity_multiplier * 255));
            }
        };
    }

    void fade_out(elements::EffectElement & fx_element, const std::chrono::milliseconds & ms)
    {
        elements::EventloopAdapter& _ev_adapter = elements::current_eventloop_adapter();
        Eventloop::clock::time_point _start_time = _ev_adapter.now();
        Eventloop::clock::time_point _end_time = _start_time + ms;

        fx_element.modify_texture_function() = [=, &fx_element, &_ev_adapter](Texture& texture) -> void
        {
            if (_ev_adapter.now() >= _end_time)
            {
                if (texture.blend_mode() == SDL_BLENDMODE_BLEND) texture.blend_mode(SDL_BLENDMODE_NONE);
                fx_element.modify_texture_function() = nullptr;
                return;
            }
            else
            {
                if (texture.blend_mode() == SDL_BLENDMODE_NONE) texture.blend_mode(SDL_BLENDMODE_BLEND);
                std::chrono::duration<double, std::milli> _duration = _ev_adapter.now() - _start_time;
                double _opacity_multiplier = 1.0 - (_duration / ms);
                texture.alpha_multiplier(static_cast<Uint8>(_opacity_multiplier * 255));
            }
        };
    }
}