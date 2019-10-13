#ifndef TSDL_DISPLAY_
#define TSDL_DISPLAY_

#include <SDL.h>
#include "TSDL_Utility.hpp"

namespace TSDL
{
    int get_total_display();
    class TSDL_Display
    {
        private:
        int _index;

        public:
        TSDL_Display(int index);

        int get_index(void);

        rect get_bound(void);
    };
}

#endif