//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_EFFECTS_FADE_
#define TSDL_EFFECTS_FADE_

#include <chrono>

// START FWD_DECL
namespace TSDL
{
    class Texture;

    namespace elements
    {
        class EffectElement;
    }
}
// END FWD_DECL

namespace TSDL::effects
{
    void fade_in_now(elements::EffectElement& fx_element, const std::chrono::milliseconds& ms);
    void fade_out_now(elements::EffectElement& fx_element, const std::chrono::milliseconds& ms);

    void fade_in(elements::EffectElement& fx_element, const std::chrono::milliseconds& ms);
    void fade_out(elements::EffectElement& fx_element, const std::chrono::milliseconds& ms);
}

#endif