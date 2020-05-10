//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/TSDL_KeyboardState.hpp"

TSDL::TSDL_KeyboardState::TSDL_KeyboardState()
{
    _internal_ptr = SDL_GetKeyboardState(NULL);
}

bool TSDL::TSDL_KeyboardState::operator[](SDL_Scancode code)
{
    return static_cast<bool>(_internal_ptr[code]);
}