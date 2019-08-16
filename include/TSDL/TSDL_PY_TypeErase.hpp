#ifndef TSDL_PY_TYPEERASE_
#define TSDL_PY_TYPEERASE_

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include <tuple>
#include <new>
#include <utility>
#include "boost/variant.hpp"

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
        boost::variant<IntermediateVariants...> _underly;

        class enter_visitor : public boost::static_visitor<ContextedCls*>
        {
        public:
            template<typename T>
            ContextedCls* operator()(T w) const
            {
                return w.enter();
            }
        };

        class exit_visitor : public boost::static_visitor<void>
        {
        public:
            template<typename T>
            void operator()(T w) const
            {
                w.exit();
            }
        };

        public:
        template <typename T>
        TypeErase(T obj): _underly(obj){}

        ContextedCls* enter()
        {
            return boost::apply_visitor(enter_visitor(), _underly);
        }
        void exit()
        {
            boost::apply_visitor(exit_visitor(), _underly);
        }
    };
}

#define _PY_EXPAND_DEFINE_CONTEXTMANAGER(TSDL_NAME)                                               \
template <typename... ConstructTypes>                                                             \
class _##TSDL_NAME                                                                                \
{                                                                                                 \
    private:                                                                                      \
    TSDL::TSDL_##TSDL_NAME* _o;                                                                   \
    std::tuple<ConstructTypes...> _t;                                                             \
                                                                                                  \
    public:                                                                                       \
    _##TSDL_NAME(ConstructTypes... args): _t(std::make_tuple(args...)) {}                         \
    ~_##TSDL_NAME(){}                                                                             \
    TSDL::TSDL_##TSDL_NAME* enter()                                                               \
    {                                                                                             \
        _o = static_cast<TSDL::TSDL_##TSDL_NAME*>(::operator new(sizeof(TSDL::TSDL_##TSDL_NAME)));\
        _PY_Util::construct_in_place_from_tuple<TSDL::TSDL_##TSDL_NAME>(_o, std::move(_t));       \
        return _o;                                                                                \
    }                                                                                             \
    void exit()                                                                                   \
    {                                                                                             \
        _o->~TSDL_##TSDL_NAME();                                                                  \
        ::operator delete(_o);                                                                    \
        _o = nullptr;                                                                             \
    }                                                                                             \
};                                                                                                \

#define _PY_EXPAND_DEFINE_TYPEERASE_OPEN(TSDL_NAME) using _##TSDL_NAME##_TypeErase = _PY_TypeErase::TypeErase<TSDL::TSDL_##TSDL_NAME, 

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

#define _PY_EXPAND_DECLARE_TYPEERASE_ENTER(TSDL_NAME) TSDL::TSDL_##TSDL_NAME* _##TSDL_NAME##_enter_ctx(_##TSDL_NAME##_TypeErase &self_);

#define _PY_EXPAND_DEFINE_TYPEERASE_ENTER(NAMESPACE, TSDL_NAME) TSDL::TSDL_##TSDL_NAME* NAMESPACE::_##TSDL_NAME##_enter_ctx(_##TSDL_NAME##_TypeErase &self_){return self_.enter();}

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

#endif