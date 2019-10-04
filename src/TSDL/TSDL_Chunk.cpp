#include "TSDL/TSDL_Chunk.hpp"
#include <stdexcept>

TSDL::TSDL_Chunk::TSDL_Chunk(Mix_Chunk* ptr): TSDL_Chunk(ptr, false) {}

TSDL::TSDL_Chunk::TSDL_Chunk(Mix_Chunk* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Chunk::TSDL_Chunk(const std::string& file): _destroy(true)
{
    Mix_Chunk* _t_internal_ptr = Mix_LoadWAV(file.c_str());
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Chunk could not be loaded! SDL_Mixer_Error: " + std::string(Mix_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Chunk::~TSDL_Chunk()
{
    if(_destroy) Mix_FreeChunk(_internal_ptr);
}

TSDL::TSDL_Chunk::operator Mix_Chunk*() const
{
    return _internal_ptr;
}

int TSDL::play(const TSDL::_SDL_Chunk& chunk, int loops)
{
    return TSDL::play(-1, chunk, loops, -1);
}

int TSDL::play(int channel, const TSDL::_SDL_Chunk& chunk, int loops)
{
    return TSDL::play(channel, chunk, loops, -1);
}

int TSDL::play(const TSDL::_SDL_Chunk& chunk, int loops, int limit)
{
    return TSDL::play(-1, chunk, loops, limit);
}

int TSDL::play(int channel, const TSDL::_SDL_Chunk& chunk, int loops, int limit)
{
    int _t = Mix_PlayChannelTimed(channel, chunk, loops, limit);
    if(_t != 0)
    {
        throw std::runtime_error("Cannot play music! SDL_Mixer_Error: " + std::string(Mix_GetError()));
    }
    return _t;
}

_TSDL_EXPAND_DECLARE_MASK_MIX(TSDL, Chunk)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Chunk)

#endif