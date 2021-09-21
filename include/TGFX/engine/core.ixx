module;

#include "TGFX/macros.hpp"
#include "TGFX/common_concepts.hpp"

#include <functional>
#include <optional>
#include <stdexcept>
#include <tuple>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module tgfx :core;

export TGFX_NAMESPACE_BEGIN

struct glfw_error: public std::runtime_error {
  int error_code;

  static glfw_error make_current_error() {
    const char* desc_ptr;
    int error_code = glfwGetError(&desc_ptr);
    return glfw_error{error_code, desc_ptr};
  }

  private:
  glfw_error(int error_code, const char* description): 
    std::runtime_error(description),
    error_code(error_code)
  {}
};

struct tgfx_raii {
  static void init();

  static void uninit();

  tgfx_raii() {
    if(!glfwInit()) {
      throw glfw_error::make_current_error();
    }
  }

  ~tgfx_raii() {
    glfwTerminate();
  }
};

TGFX_NAMESPACE_END

TGFX_IMPL_BEGIN

static std::optional<tgfx_raii> _global_tgfx_raii;

TGFX_IMPL_END

export TGFX_NAMESPACE_BEGIN

void tgfx_raii::init() { 
  impl::_global_tgfx_raii.emplace(); 
}

void tgfx_raii::uninit() { 
  impl::_global_tgfx_raii.reset(); 
}

TGFX_NAMESPACE_END

TGFX_IMPL_BEGIN

template <std::size_t N>
struct _string_literal_t {
  char v[N];

  constexpr _string_literal_t(const char (&str)[N]) {
    std::copy_n(str, N, v);
  }

  template <std::size_t L>
  constexpr bool operator==(const _string_literal_t<L>& other) noexcept {
    return std::string_view{v, N} == std::string_view{other.v, L};
  }

  template <std::size_t L>
  constexpr auto operator<=>(const _string_literal_t<L>& other) noexcept {
    return std::string_view{v, N} <=> std::string_view{other.v, L};
  }
};

template <_string_literal_t key, typename FType, typename SlotTag>
struct _func_kv_node;

template <_string_literal_t _key, typename _FType, typename _SlotTag>
_func_kv_node<_key, _FType, _SlotTag> _get_node(_func_kv_node<_key, _FType, _SlotTag>);

template <_string_literal_t key, typename FType>
struct _func_kv_node<key, FType, std::nullptr_t> {
  using function_type = FType;
  using slot_tag = std::nullptr_t;
  using handler_always_exist = std::false_type;

  _func_kv_node() = default;
  template <typename F>
  _func_kv_node(F&& f): f{std::move(f)} {}

  private:
  std::function<FType> f;

  template <_string_literal_t _key, typename _FType>
  friend std::function<_FType>& get(_func_kv_node<_key, _FType, std::nullptr_t>&);
  template <_string_literal_t _key, typename _FType>
  friend const std::function<_FType>& get(const _func_kv_node<_key, _FType, std::nullptr_t>&);
  template <_string_literal_t _key, typename _FType>
  friend std::function<_FType>&& get(_func_kv_node<_key, _FType, std::nullptr_t>&&);
  template <_string_literal_t _key, typename _FType>
  friend const std::function<_FType>&& get(const _func_kv_node<_key, _FType, std::nullptr_t>&&);
};

template <_string_literal_t _key, typename _FType>
std::function<_FType>& get(_func_kv_node<_key, _FType, std::nullptr_t>& node) {
  return node.f;
}

template <_string_literal_t _key, typename _FType>
const std::function<_FType>& get(const _func_kv_node<_key, _FType, std::nullptr_t>& node) {
  return node.f;
}

template <_string_literal_t _key, typename _FType>
std::function<_FType>&& get(_func_kv_node<_key, _FType, std::nullptr_t>&& node) {
  return std::move(node.f);
}

template <_string_literal_t _key, typename _FType>
const std::function<_FType>&& get(const _func_kv_node<_key, _FType, std::nullptr_t>&& node) {
  return std::move(node.f);
}

TGFX_IMPL_END

export TGFX_NAMESPACE_BEGIN

template <typename StaticFuncType>
struct static_function_tag_t {};

template <typename StaticFuncType>
static constexpr static_function_tag_t<StaticFuncType> static_function_tag{};

TGFX_NAMESPACE_END

TGFX_IMPL_BEGIN

template <_string_literal_t key, typename FType, typename F>
struct _func_kv_node<key, FType, static_function_tag_t<F>> {
  using function_type = FType;
  using slot_tag = static_function_tag_t<F>;
  using handler_always_exist = std::true_type;

  _func_kv_node(F&& f): f{std::move(f)} {}

  private:
  F f;

  template <_string_literal_t _key, typename _FType, typename _F>
  friend const _F& get(const _func_kv_node<_key, _FType, static_function_tag_t<_F>>&);
};

template <_string_literal_t _key, typename _FType, typename _F>
const _F& get(const _func_kv_node<_key, _FType, static_function_tag_t<_F>>& node) {
  return node.f;
}

TGFX_IMPL_END

export TGFX_NAMESPACE_BEGIN

template <typename ClassWithMemberFnType, typename ClassType = ClassWithMemberFnType>
struct static_member_function_tag_t {};

template <typename ClassWithMemberFnType, typename ClassType = ClassWithMemberFnType>
static constexpr static_member_function_tag_t<ClassWithMemberFnType, ClassType> static_member_function_tag{};

TGFX_NAMESPACE_END

TGFX_IMPL_BEGIN

template <_string_literal_t key, typename RetType, typename ClassWithMemberFnType, typename ClassType, typename... Args>
struct _func_kv_node<key, RetType(Args...), static_member_function_tag_t<ClassWithMemberFnType, ClassType>> {
  using function_type = RetType(Args...);
  using slot_tag = static_member_function_tag_t<ClassWithMemberFnType, ClassType>;
  using handler_always_exist = std::true_type;

  using member_function_type = RetType (ClassWithMemberFnType::*) (Args...);

  _func_kv_node(member_function_type&& f): f{std::move(f)} {}

  auto _get_fn() {
    return std::bind_front(f, static_cast<ClassWithMemberFnType&>(static_cast<ClassType&>(*this)));
  }

  private:
  member_function_type f;
};

template <_string_literal_t _key, typename _RetType, typename _ClassWithMemberFnType, typename _ClassType, typename... _Args>
auto get(_func_kv_node<_key, _RetType(_Args...), static_member_function_tag_t<_ClassWithMemberFnType, _ClassType>>& node) {
  return node._get_fn();
}

template <std::size_t N, typename FType, typename SlotTag = std::nullptr_t>
struct _event_desc_t {
  _string_literal_t<N> name;
  using function_type = FType;
  using slot_tag = SlotTag;
  
  constexpr _event_desc_t(const char (&name)[N], std::type_identity<FType> func_type_holder): name{name} {}
  constexpr _event_desc_t(const char (&name)[N], std::type_identity<FType> func_type_holder, SlotTag): name{name} {}
};

template <_string_literal_t event, typename F>
struct _event_default_t {
  std::remove_cvref_t<F> f;
  
  constexpr _event_default_t(F&& f): f(std::forward<F>(f)) {}
};

template <_event_desc_t... eventdesc>
struct _func_kv: public _func_kv_node<
  eventdesc.name, 
  typename decltype(eventdesc)::function_type, 
  typename decltype(eventdesc)::slot_tag
>... {
  template <_string_literal_t... events, typename... Fs>
  constexpr _func_kv(_event_default_t<events, Fs>&&... defaults): 
    _func_kv_node<
      events, 
      typename decltype(_get_node<events>(std::declval<_func_kv>()))::function_type, 
      typename decltype(_get_node<events>(std::declval<_func_kv>()))::slot_tag
    >{std::move(defaults.f)}... 
  {}
};

TGFX_IMPL_END

export TGFX_NAMESPACE_BEGIN

template <impl::_string_literal_t event, typename F>
impl::_event_default_t<event, F> make_event_default(F&& f) {
  return impl::_event_default_t<event, F>{std::forward<F>(f)};
}

template <impl::_event_desc_t... eventdesc>
struct event_emitter: public impl::_func_kv<eventdesc...> {
  using emitter_type = event_emitter<eventdesc...>;

  template <impl::_string_literal_t event>
  using function_result_type = typename std::function<
    typename decltype(
      _get_node<event>(std::declval<impl::_func_kv<eventdesc...>>())
    )::function_type
  >::result_type;

  template <impl::_string_literal_t event>
  using function_dereferenced_return_type = std::remove_reference_t<function_result_type<event>>;

  template <impl::_string_literal_t event>
  using function_wrapped_ref_return_type = std::reference_wrapper<function_dereferenced_return_type<event>>;

  template <typename... _default_ts>
  constexpr event_emitter(_default_ts&&... defaults) :
    impl::_func_kv<eventdesc...>{std::forward<_default_ts>(defaults)...}
  {}

  constexpr event_emitter() = default;

  template <impl::_string_literal_t event, typename F>
  void set_handler(F&& f) {
    impl::get<event>(*this) = std::forward<F>(f);
  }

  template <impl::_string_literal_t event, typename F>
  void on(F&& f) {
    set_handler(std::forward<F>(f));
  }

  template <impl::_string_literal_t event>
  decltype(auto) get_handler() {
    return impl::get<event>(*this);
  }

  template <impl::_string_literal_t event>
  bool has_handler() {
    auto& handler = get_handler<event>();
    if constexpr (concepts::specialization_of<std::remove_cvref_t<decltype(handler)>, std::function>) {
      return static_cast<bool>(handler);
    }
    else return true;
  }

  template <impl::_string_literal_t event, typename... Args>
  decltype(auto) force_emit(Args&&... args) {
    auto& handler = get_handler<event>();
    return [&handler]() -> function_result_type<event> { return handler(std::forward<Args>(args)...); }();
  }

  /**
   * @brief Call handler associated with `event` and get its return result if possible.
   * 
   * Try to call handler associated with `event`. 
   * If handler is always present for the slot, the result is simply returned.
   * If handler return void then return type is bool.
   * If handler return lvalue then return type is std::optional<std::reference_wrapper<std::remove_reference_t<**func_return_type**>>>.
   * If handler return not lvalue then return type is std::optional<std::remove_reference_t<**func_return_type**>>.
   */
  template <impl::_string_literal_t event, typename... Args>
  decltype(auto) emit(Args&&... args) {
    decltype(auto) handler = get_handler<event>();
    if constexpr (!decltype(impl::_get_node<event>(*this))::handler_always_exist::value) {
      if constexpr (std::same_as<function_result_type<event>, void>) {
        if(!handler) return false;
        else {
          handler(std::forward<Args>(args)...);
          return true;
        }
      }
      else if constexpr (std::is_lvalue_reference_v<function_result_type<event>>) {
        if(!handler) return std::optional<function_wrapped_ref_return_type<event>>{};
        else return std::optional<function_wrapped_ref_return_type<event>>{
          function_wrapped_ref_return_type<event>{handler(std::forward<Args>(args)...)}
        };
      }
      else {
        if(!handler) return std::optional<function_dereferenced_return_type<event>>{};
        else return std::optional<function_dereferenced_return_type<event>>{
          handler(std::forward<Args>(args)...)
        };
      }
    }
    else if constexpr (std::same_as<function_result_type<event>, void>) {
      handler(std::forward<Args>(args)...);
      return;
    }
    else return function_result_type<event>{handler(std::forward<Args>(args)...)};
  }
};

template <typename T>
struct is_event_emitter_specialization : public std::false_type {};

template <impl::_event_desc_t... Args>
struct is_event_emitter_specialization<event_emitter<Args...>> : public std::true_type {};

template <typename T>
static constexpr bool is_event_emitter_specialization_v = is_event_emitter_specialization<T>::value;

template <typename T>
concept specialization_of_event_emitter = is_event_emitter_specialization_v<T>;

template <typename T, impl::_string_literal_t event, typename F>
concept has_event_slot = requires {
  requires specialization_of_event_emitter<T>;
  requires std::same_as<typename T::template function_result_type<event>, F>;
};

TGFX_NAMESPACE_END
