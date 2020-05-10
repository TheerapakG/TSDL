//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/TSDL_Display.hpp"
#include "TSDL/TSDL_Utility.hpp"

TSDL::TSDL_Display::TSDL_Display(int index): _index(index) {};

int TSDL::TSDL_Display::index(void) const
{
    return _index;
}

TSDL::rect TSDL::TSDL_Display::bound(void) const
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