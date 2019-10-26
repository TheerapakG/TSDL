#ifndef TSDL_KEYBOARDSTATE_
#define TSDL_KEYBOARDSTATE_

#include <SDL.h>

namespace TSDL
{
    class TSDL_KeyboardState
    {
        private:
        const Uint8* _internal_ptr;

        public:
        TSDL_KeyboardState();

        bool operator[](SDL_Scancode code);

    };
}

#endif