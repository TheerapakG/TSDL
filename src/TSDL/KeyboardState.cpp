//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/KeyboardState.hpp"

TSDL::KeyboardState::KeyboardState()
{
    _internal_ptr = SDL_GetKeyboardState(NULL);
}

bool TSDL::KeyboardState::operator[](SDL_Scancode code)
{
    return static_cast<bool>(_internal_ptr[code]);
}