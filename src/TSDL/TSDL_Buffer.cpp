#include "TSDL/TSDL_Buffer.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include <limits>
#include <string>
#include <iostream>

TSDL_DEFINE_CONSTRUCT(TSDL, Buffer)

TSDL::TSDL_Buffer::TSDL_Buffer(SDL_RWops* ptr): TSDL_Buffer(ptr, false) {}

TSDL::TSDL_Buffer::TSDL_Buffer(SDL_RWops* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Buffer::TSDL_Buffer(const std::string& file, const std::string& mode)
{
    SDL_RWops* _t_internal_ptr = SDL_RWFromFile(file.c_str(), mode.c_str());
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

TSDL::TSDL_Buffer::TSDL_Buffer(void* mem, size_t size)
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

TSDL::TSDL_Buffer::TSDL_Buffer(const void* mem, size_t size)
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

TSDL::TSDL_Buffer::~TSDL_Buffer()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_RWclose(_internal_ptr);
}

TSDL::TSDL_Buffer::operator SDL_RWops*() const
{
    return _internal_ptr;
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Buffer)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Buffer)

void _tsdl_buffer_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Buffer)>(m, "Buffer")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Buffer)<const std::string, const std::string>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Buffer, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Buffer, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Buffer, exit_ctx));
    py::class_<TSDL::TSDL_Buffer>(m, "_Buffer");
    py::class_<TSDL::_TSDL_GET_MASK_TYPE(Buffer)>(m, "_SDL_Buffer");
    py::implicitly_convertible<TSDL::TSDL_Buffer, TSDL::_TSDL_GET_MASK_TYPE(Buffer)>();
}

#endif