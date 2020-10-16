//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Chunk.hpp"
#include "TSDL/Utility.hpp"

TSDL_DEFINE_CONSTRUCT(TSDL, Chunk)

TSDL::Chunk::Chunk(Mix_Chunk* ptr): Chunk(ptr, false) {}

TSDL::Chunk::Chunk(Mix_Chunk* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::Chunk::Chunk(const std::_TSDL_U8(string)& file): _destroy(true)
{
    Mix_Chunk* _t_internal_ptr = Mix_LoadWAV(reinterpret_cast<const char*>(file.c_str()));
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Chunk could not be loaded! SDL_Mixer_Error: " + std::string(Mix_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

Mix_Chunk* _create_chunk_from_buffer(SDL_RWops* buffer)
{
    Mix_Chunk* _t_ptr = Mix_LoadWAV_RW(buffer, 0); // We free buffer by utilizing the scope
    if(_t_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Chunk could not be loaded! SDL_Mixer_Error: " + std::string(Mix_GetError()));
        return nullptr;
    }
    return _t_ptr;
}

TSDL::Chunk::Chunk(TSDL::Buffer& buffer)
{
    Mix_Chunk* _t_internal_ptr = _create_chunk_from_buffer(buffer);
    _internal_ptr = _t_internal_ptr;
}

TSDL::Chunk::Chunk(const void* mem, size_t size)
{
    TSDL::Buffer _t_buffer = TSDL::Buffer(mem, size);
    Mix_Chunk* _t_internal_ptr = _create_chunk_from_buffer(_t_buffer);
    _internal_ptr = _t_internal_ptr;
}

TSDL::Chunk::~Chunk()
{
    if(_destroy && (_internal_ptr != nullptr)) Mix_FreeChunk(*this);
}

TSDL::Chunk::operator Mix_Chunk*() const
{
    return _internal_ptr;
}

int TSDL::play(const TSDL::Chunk& chunk, int loops)
{
    return TSDL::play(-1, chunk, loops, -1);
}

int TSDL::play(int channel, const TSDL::Chunk& chunk, int loops)
{
    return TSDL::play(channel, chunk, loops, -1);
}

int TSDL::play(const TSDL::Chunk& chunk, int loops, int limit)
{
    return TSDL::play(-1, chunk, loops, limit);
}

int TSDL::play(int channel, const TSDL::Chunk& chunk, int loops, int limit)
{
    int _t = Mix_PlayChannelTimed(channel, chunk, loops, limit);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Cannot play music! SDL_Mixer_Error: " + std::string(Mix_GetError()));
    }
    return _t;
}

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Chunk)

void _tsdl_chunk_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Chunk)>(m, "Chunk")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Chunk)<const std::_TSDL_U8(string)>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Chunk, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Chunk, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Chunk, exit_ctx));
    py::class_<TSDL::Chunk>(m, "_Chunk");
}

#endif