#include "TSDL/TSDL_KeyboardState.hpp"

TSDL::TSDL_KeyboardState::TSDL_KeyboardState()
{
    _internal_ptr = SDL_GetKeyboardState(NULL);
}

bool TSDL::TSDL_KeyboardState::operator[](SDL_Scancode code)
{
    return static_cast<bool>(_internal_ptr[code]);
}