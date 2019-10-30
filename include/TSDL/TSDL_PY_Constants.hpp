#ifndef TSDL_PY_CONSTANTS_
#define TSDL_PY_CONSTANTS_

#ifdef TSDL_EXPOSE_PYBIND11

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include <SDL.h>

void _tsdl_py_constant_py(const py::module& m);

#endif

#endif