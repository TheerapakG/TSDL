#ifndef TSDL_MAIN_
#define TSDL_MAIN_

namespace TSDL
{
    class TSDL
    {
        public:
        TSDL();
        TSDL(int frequency);
        ~TSDL();
    };
}

#ifdef TSDL_EXPOSE_PYBIND11

#define TSDL_TSDL TSDL

#include "TSDL_PY_TypeErase.hpp"

namespace _PY
{
    _PY_EXPAND_DEFINE_CONTEXTMANAGER(TSDL)

    _PY_EXPAND_DEFINE_TYPEERASE_OPEN(TSDL)
    _PY_GET_CONTEXTMANAGER(TSDL)<>
    _PY_EXPAND_DEFINE_TYPEERASE_CLOSE

    _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(TSDL)
}

#define _TSDL_MAIN_PY                                                                                                 \
    py::class_<_PY::_PY_GET_TYPEERASE(TSDL)>(m, "TSDL")                                                               \
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(TSDL)<>())                                                                \
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(TSDL, enter_ctx), py::return_value_policy::reference)      \
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(TSDL, exit_ctx));                                           \
    py::class_<TSDL::TSDL>(m, "_TSDL")                                                                                \
        .def(py::init<>());                                                                                           \

#undef TSDL_TSDL

#endif

#endif