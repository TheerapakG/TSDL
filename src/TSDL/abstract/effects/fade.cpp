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
        TSDL_Eventloop::clock::time_point _start_time = _ev_adapter.now();
        TSDL_Eventloop::clock::time_point _end_time = _start_time + ms;

        fx_element.modify_texture_function(
            [&](TSDL_Texture& texture) -> void
            {
                if(_ev_adapter.now() >= _end_time)
                {
                    fx_element.modify_texture_function();
                    return;
                }
                else
                {
                    std::chrono::duration<double, std::milli> _duration = _ev_adapter.now()-_start_time;
                    double _opacity_multiplier = _duration/ms;
                    texture.alpha_multiplier(255 * _opacity_multiplier);
                }                               
            }
        );
    }
}