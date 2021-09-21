module;

#include "TGFX/macros.hpp"
#include "TGFX/common_concepts.hpp"

#include <compare>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module tgfx :window;

import :core;
import :monitor;

export TGFX_NAMESPACE_BEGIN

TGFX_NAMESPACE_END

struct window;

TGFX_IMPL_BEGIN

std::vector<std::pair<int, int>> default_window_hints = {
  {GLFW_CLIENT_API, GLFW_NO_API}
};

std::vector<std::pair<int, std::string_view>> default_window_hints_string = {};

struct _window_base {
  template <std::convertible_to<std::shared_ptr<GLFWwindow>> _shared_ptr_t>
  explicit _window_base(_shared_ptr_t&& ptr) : _internal_ptr{std::forward<_shared_ptr_t>(ptr)} {}

  explicit operator GLFWwindow*() { 
    auto ptr = _internal_ptr.get(); 
    if (ptr != nullptr) [[likely]] return ptr;
    else [[unlikely]] return NULL;
  }

  void _on_change_pos(int xpos, int ypos) {
    glfwSetWindowPos(static_cast<GLFWwindow*>(*this), xpos, ypos);
  }

  void _on_change_size(int width, int height) {
    glfwSetWindowSize(static_cast<GLFWwindow*>(*this), width, height);
  }

  void _on_should_close() {
    glfwWindowShouldClose(static_cast<GLFWwindow*>(*this));
  }

  void _on_change_focus() {
    glfwFocusWindow(static_cast<GLFWwindow*>(*this));
  }

  void _on_iconify() {
    glfwIconifyWindow(static_cast<GLFWwindow*>(*this));
  }

  private:  
  std::shared_ptr<GLFWwindow> _internal_ptr;

  friend struct std::hash<_window_base>;
};

TGFX_IMPL_END

export TGFX_NAMESPACE_BEGIN

struct window: public impl::_window_base,
public event_emitter<
  {
    "change_pos", std::type_identity<void(int, int)>{}, 
    static_member_function_tag<impl::_window_base, window>
  }, 
  {
    "change_size", std::type_identity<void(int, int)>{}, 
    static_member_function_tag<impl::_window_base, window>
  }, 
  {
    "should_close", std::type_identity<void(void)>{}, 
    static_member_function_tag<impl::_window_base, window>
  }, 
  {
    "refresh", std::type_identity<void(void)>{}
  }, 
  {
    "change_focus", std::type_identity<void(void)>{}, 
    static_member_function_tag<impl::_window_base, window>
  }, 
  {
    "iconify", std::type_identity<void(void)>{}, 
    static_member_function_tag<impl::_window_base, window>
  }
> {  
  template <
    concepts::ranges::input_ranges_convertible_to<
      std::pair<int, int>> _input_window_hints_t = std::initializer_list<std::pair<int, int>>, 
    concepts::ranges::input_ranges_convertible_to<
      std::pair<int, std::string_view>> _input_window_hints_string_t = std::initializer_list<std::pair<int, std::string_view>>
  >
  static window create_window(
    int width, 
    int height, 
    const std::string& title, 
    _input_window_hints_t&& window_hints = {},
    _input_window_hints_string_t&& window_hints_string = {},
    const std::optional<const std::reference_wrapper<monitor>>& monitor_ = std::nullopt, 
    const std::optional<const std::reference_wrapper<window>>& share = std::nullopt
  ) {
    window::reset_hint();
    for (auto&& [hint, value]: impl::default_window_hints) window::set_hint(hint, value);
    for (auto&& [hint, value]: impl::default_window_hints_string) window::set_hint(hint, value);

    for (auto [hint, value]: window_hints) window::set_hint(hint, value); // stealing ints are not worth it

    if constexpr (std::is_rvalue_reference_v<_input_window_hints_string_t>) {
      for (auto&& [hint, value]: window_hints_string) window::set_hint(hint, std::move(value));
    }
    else for (auto&& [hint, value]: window_hints_string) window::set_hint(hint, value);

    GLFWwindow* ptr = glfwCreateWindow(
      width, 
      height, 
      title.c_str(), 
      monitor_ ? static_cast<GLFWmonitor*>(monitor_.value().get()) : nullptr, 
      share ? static_cast<GLFWwindow*>(share.value().get()) : nullptr
    );
    if (ptr == NULL) [[unlikely]] throw glfw_error::make_current_error();

    return window{std::shared_ptr<GLFWwindow>{ptr, glfwDestroyWindow}};
  }

  explicit window(std::nullptr_t) : window{std::shared_ptr<GLFWwindow>{}} {}

  template <std::convertible_to<std::shared_ptr<GLFWwindow>> _shared_ptr_t>
  explicit window(_shared_ptr_t&& ptr) : 
    _window_base{std::forward<_shared_ptr_t>(ptr)},
    emitter_type{
      make_event_default<"change_pos">(&window::_on_change_pos),
      make_event_default<"change_size">(&window::_on_change_size),
      make_event_default<"should_close">(&window::_on_should_close),
      make_event_default<"change_focus">(&window::_on_change_focus),
      make_event_default<"iconify">(&window::_on_iconify)
    }
  {}

  explicit window(GLFWwindow* ptr) : window{std::shared_ptr<GLFWwindow>{std::shared_ptr<GLFWwindow>{}, ptr}} {}

  std::strong_ordering operator<=>(const window&) const = default;

  static void set_hint(int hint, int value) {
    glfwWindowHint(hint, value);
  }
  template <std::convertible_to<std::string_view> _string_view>
  static void set_hint(int hint, _string_view&& value) {
    glfwWindowHintString(hint, std::string(std::forward<_string_view>(value)).c_str());
  }
  static void reset_hint() {
    glfwDefaultWindowHints();
  }

  bool should_close() {
    return glfwWindowShouldClose(static_cast<GLFWwindow*>(*this));
  }
  void should_close(bool value) {
    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(*this), value);
  }
};

TGFX_NAMESPACE_END

TGFX_STD_BEGIN

template <> 
struct hash<tgfx::impl::_window_base> {
  std::size_t operator()(const tgfx::impl::_window_base& window) const noexcept {
    return hash<std::shared_ptr<GLFWwindow>>{}(window._internal_ptr);
  }
};

template <> 
struct hash<tgfx::window> {
  std::size_t operator()(const tgfx::window& window) const noexcept {
    return hash<tgfx::impl::_window_base>{}(window);
  }
};

TGFX_STD_END
