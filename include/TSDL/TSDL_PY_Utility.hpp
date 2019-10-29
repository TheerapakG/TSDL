#ifndef TSDL_PY_UTILITY_
#define TSDL_PY_UTILITY_

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    template <typename T, std::size_t... I>
    T _py_function_cast_function_pointer_gen_lambda(py::function& func, std::index_sequence<I...>)
    {
        static py::function f = func;
        return [](get_argument_t<T, I>... args)
        {
            if (std::is_same_v<function_traits<T>::return_type, void>) f(args...);
            else return f(args...).template cast<typename function_traits<T>::return_type>();
        };
    }

    template <typename T, typename Indices = std::make_index_sequence<function_traits<T>::arity>>
    T py_function_cast_function_pointer(py::function func)
    {
        static_assert(is_callable_not_overloaded_v<T>, "T is not callable or is ambiguous");
        return _py_function_cast_function_pointer_gen_lambda<T>(func, Indices {});
    }
}

#endif