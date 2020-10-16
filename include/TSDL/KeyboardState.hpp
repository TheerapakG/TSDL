//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_KEYBOARDSTATE_
#define TSDL_KEYBOARDSTATE_

#include <SDL2/SDL.h>

namespace TSDL
{
    class KeyboardState
    {
        private:
        const Uint8* _internal_ptr;

        public:
        KeyboardState();

        bool operator[](SDL_Scancode code);

    };
}

#endif