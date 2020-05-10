//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_DISPLAY_
#define TSDL_DISPLAY_

#include <SDL2/SDL.h>
#include "TSDL_Utility.hpp"

namespace TSDL
{
    int total_display();
    class TSDL_Display
    {
        private:
        int _index;

        public:
        TSDL_Display(int index);

        int index(void) const;

        rect bound(void) const;
    };
}

#endif