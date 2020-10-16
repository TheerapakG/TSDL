//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/elements/EffectElement.hpp"
#include "TSDL/abstract/elements/WindowAdapter.hpp"
#include "TSDL/Meta.hpp"

namespace TSDL::elements
{
    EffectElement::TextureModifyFunc& EffectElement::modify_texture_function()
    {
        return _texture_modifier;
    }
    
    const EffectElement::TextureModifyFunc& EffectElement::modify_texture_function() const
    {
        return _texture_modifier;
    }
}