//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Buffer.hpp"
#include "TSDL/Utility.hpp"
#include <limits>
#include <string>
#include <iostream>

TSDL_DEFINE_CONSTRUCT(TSDL, Buffer)

TSDL::Buffer::Buffer(SDL_RWops* ptr): Buffer(ptr, false) {}

TSDL::Buffer::Buffer(SDL_RWops* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::Buffer::Buffer(const std::_TSDL_U8(string)& file, const std::string& mode)
{
    SDL_RWops* _t_internal_ptr = SDL_RWFromFile(reinterpret_cast<const char*>(file.c_str()), mode.c_str());
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Buffer could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

namespace
{
    inline void _patch_mem_rwops_size(SDL_RWops* ctx, size_t size) noexcept
    {
        // Patch size in case that size is larger than int
        ctx->hidden.mem.stop = ctx->hidden.mem.base + size;
    }
}

TSDL::Buffer::Buffer(void* mem, size_t size)
{
    SDL_RWops* _t_internal_ptr = SDL_RWFromMem(mem, static_cast<int>(size));
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Buffer could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _patch_mem_rwops_size(_internal_ptr, size);
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

TSDL::Buffer::Buffer(const void* mem, size_t size)
{
    SDL_RWops* _t_internal_ptr = SDL_RWFromConstMem(mem, static_cast<int>(size));
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Buffer could not be created! SDL_Error: " + std::string(SDL_GetError()));
        return;
    }
    _patch_mem_rwops_size(_internal_ptr, size);
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

TSDL::Buffer::~Buffer()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_RWclose(_internal_ptr);
}

TSDL::Buffer::operator SDL_RWops*() const
{
    return _internal_ptr;
}

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Buffer)

void _tsdl_buffer_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Buffer)>(m, "Buffer")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Buffer)<const std::_TSDL_U8(string), const std::string>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Buffer, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Buffer, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Buffer, exit_ctx));
    py::class_<TSDL::Buffer>(m, "_Buffer");
}

#endif