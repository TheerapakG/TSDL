//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_PY_TYPEERASE_
#define TSDL_PY_TYPEERASE_

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include <tuple>
#include <new>
#include <utility>
#include <variant>
#include <stdexcept>

namespace _PY_Util
{
    template<class T, class Tuple, size_t... Indices>
    void _construct_in_place_from_tuple(T* ptr, Tuple&& Tpl, std::integer_sequence<size_t, Indices...>)
    {
        new (ptr) T(std::get<Indices>(std::forward<Tuple>(Tpl))...);
    }

    template<class T, class Tuple>
    void construct_in_place_from_tuple(T* ptr, Tuple&& Tpl)
    {
        _construct_in_place_from_tuple<T>
        (
            ptr, 
            std::forward<Tuple>(Tpl), 
            std::make_integer_sequence<size_t, std::tuple_size_v<std::remove_reference_t<Tuple>>>{}
        );
    }
}

namespace _PY_TypeErase
{
    template<typename ContextedCls, typename... IntermediateVariants>
    class TypeErase
    {
        private:
        std::variant<IntermediateVariants...> _underly;

        public:
        template <typename T>
        TypeErase(T obj): _underly(obj){}

        ContextedCls* enter()
        {
            ContextedCls* ret = std::visit([](auto&& arg){ return arg.enter(); }, _underly);
            ret->_owner = this;
            return ret;
        }
        void exit()
        {
            std::visit([](auto&& arg){ return arg.exit(); }, _underly);
        }
    };
}

#define _PY_EXPAND_DECLARE_CONTEXTMANAGER(TSDL_NAME)      \
template <typename... ConstructTypes>                     \
class _##TSDL_NAME;                                       \

#define _PY_EXPAND_DEFINE_CONTEXTMANAGER(TSDL_NAME)                                               \
template <typename... ConstructTypes>                                                             \
class _##TSDL_NAME                                                                                \
{                                                                                                 \
    private:                                                                                      \
    bool constructed = false;                                                                     \
    TSDL::TSDL_NAME* _o = nullptr;                                                                \
    std::tuple<ConstructTypes...> _t;                                                             \
                                                                                                  \
    public:                                                                                       \
    _##TSDL_NAME(ConstructTypes... args): _t(std::make_tuple(args...)) {}                         \
    ~_##TSDL_NAME(){}                                                                             \
    TSDL::TSDL_NAME* enter()                                                                      \
    {                                                                                             \
        if(constructed) throw std::runtime_error("Object has already been made!");                \
        _o = ::operator new(sizeof(TSDL::TSDL_NAME), alignof(TSDL::TSDL_NAME));                   \
        try                                                                                       \
        {                                                                                         \
            _PY_Util::construct_in_place_from_tuple<TSDL::TSDL_NAME>(_o, std::move(_t));          \
            constructed = true;                                                                   \
        }                                                                                         \
        catch (...)                                                                               \
        {                                                                                         \
            ::operator delete(_o);                                                                \
            _o = nullptr;                                                                         \
            throw;                                                                                \
        }                                                                                         \
        return _o;                                                                                \
    }                                                                                             \
    void exit()                                                                                   \
    {                                                                                             \
        if(_o != nullptr)                                                                         \
        {                                                                                         \
            delete _o;                                                                            \
            _o = nullptr;                                                                         \
        }                                                                                         \
    }                                                                                             \
};                                                                                                \

#define _PY_EXPAND_DECLARE_CLASS(TSDL_NAME)     \
namespace TSDL                                  \
{                                               \
    class TSDL_NAME;                            \
}                                               \

#define _PY_EXPAND_DEFINE_TYPEERASE_OPEN(TSDL_NAME) using _##TSDL_NAME##_TypeErase = _PY_TypeErase::TypeErase<TSDL::TSDL_NAME, 

#define _PY_GET_CONTEXTMANAGER(TSDL_NAME) _##TSDL_NAME

#define _PY_EXPAND_DEFINE_TYPEERASE_CLOSE >;

#define __PY_EXPAND_DEFINE_TYPEERASE_PY_INIT(TypeErase_T, Erased_T)                     \
template<typename... IntermediateArgs>                                                  \
TypeErase_T _py_init_func_##TypeErase_T##_##Erased_T(IntermediateArgs... args)          \
{                                                                                       \
    return TypeErase_T(Erased_T<IntermediateArgs...>(args...));                         \
}                                                                                       \
                                                                                        \
template<typename... IntermediateArgs>                                                  \
auto py_init_##TypeErase_T##_##Erased_T()                                               \
{                                                                                       \
    return py::init(&_py_init_func_##TypeErase_T##_##Erased_T<IntermediateArgs...>);    \
}                                                                                       \

#define _PY_EXPAND_DEFINE_TYPEERASE_PY_INIT(TSDL_NAME) __PY_EXPAND_DEFINE_TYPEERASE_PY_INIT(_##TSDL_NAME##_TypeErase, _##TSDL_NAME)

#define _PY_EXPAND_DECLARE_TYPEERASE_ENTER(TSDL_NAME) TSDL::TSDL_NAME* _##TSDL_NAME##_enter_ctx(_##TSDL_NAME##_TypeErase &self_);

#define _PY_EXPAND_DEFINE_TYPEERASE_ENTER(NAMESPACE, TSDL_NAME) TSDL::TSDL_NAME* NAMESPACE::_##TSDL_NAME##_enter_ctx(_##TSDL_NAME##_TypeErase &self_){return self_.enter();}

#define _PY_EXPAND_DECLARE_TYPEERASE_EXIT(TSDL_NAME) void _##TSDL_NAME##_exit_ctx(_##TSDL_NAME##_TypeErase &self_, py::args args);

#define _PY_EXPAND_DEFINE_TYPEERASE_EXIT(NAMESPACE, TSDL_NAME) void NAMESPACE::_##TSDL_NAME##_exit_ctx(_##TSDL_NAME##_TypeErase &self_, py::args args){self_.exit();}

#define _PY_EXPAND_DECLARE_TYPEERASE_FUNCTIONS(TSDL_NAME) \
_PY_EXPAND_DEFINE_TYPEERASE_PY_INIT(TSDL_NAME)            \
_PY_EXPAND_DECLARE_TYPEERASE_ENTER(TSDL_NAME)             \
_PY_EXPAND_DECLARE_TYPEERASE_EXIT(TSDL_NAME)              \

#define _PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(NAMESPACE, TSDL_NAME) \
_PY_EXPAND_DEFINE_TYPEERASE_ENTER(NAMESPACE, TSDL_NAME)             \
_PY_EXPAND_DEFINE_TYPEERASE_EXIT(NAMESPACE, TSDL_NAME)              \

#define __PY_GET_TYPEERASE_PY_INIT(TypeErase_T, Erased_T) py_init_##TypeErase_T##_##Erased_T

#define _PY_GET_TYPEERASE_PY_INIT(TSDL_NAME) __PY_GET_TYPEERASE_PY_INIT(_##TSDL_NAME##_TypeErase, _##TSDL_NAME)

#define _PY_GET_TYPEERASE_FUNCTION(TSDL_NAME, function) _##TSDL_NAME##_##function

#define _PY_GET_TYPEERASE(TSDL_NAME) _##TSDL_NAME##_TypeErase

#define _PY_DECLARE_TYPEERASE_OWNER(TSDL_NAME) _PY::_PY_GET_TYPEERASE(TSDL_NAME)* _owner = nullptr;

#endif