#ifndef TSDL_PY_CONSTANTS_
#define TSDL_PY_CONSTANTS_

#ifdef TSDL_EXPOSE_PYBIND11

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include <SDL.h>

#define _TSDL_PY_EXPAND_OR_ENUM(T) [](T _self, T _other){ return static_cast<T>(_self|_other); }

void _tsdl_py_constant_py(const py::module& m);

#endif

#endif