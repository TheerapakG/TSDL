//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Display.hpp"
#include "TSDL/Utility.hpp"

TSDL::Display::Display(int index): _index(index) {};

int TSDL::Display::index(void) const
{
    return _index;
}

TSDL::rect TSDL::Display::bound(void) const
{
    SDL_Rect _rect;
    int _t = SDL_GetDisplayBounds(_index, &_rect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not get display bound! SDL Error: " + std::string(SDL_GetError()));
        // TODO: noexcept signify error
    }
    return _rect;
}