#ifndef TSDL_MAIN_
#define TSDL_MAIN_

namespace TSDL
{
    class TSDL
    {
        public:
        #ifndef __cpp_exceptions
        bool constructed = false;
        #endif
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

void _tsdl_main_py(const py::module& m);

#undef TSDL_TSDL

#endif

#endif