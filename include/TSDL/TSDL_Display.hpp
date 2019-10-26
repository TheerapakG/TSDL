#ifndef TSDL_DISPLAY_
#define TSDL_DISPLAY_

#include <SDL.h>
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

        int index(void);

        rect bound(void);
    };
}

#endif