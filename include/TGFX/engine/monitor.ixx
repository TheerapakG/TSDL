module;

#include "TGFX/macros.hpp"

#include <functional>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module tgfx :monitor;

import :core;

export TGFX_NAMESPACE_BEGIN

struct monitor {
  static std::vector<monitor> get_monitors() {
    int monitors_count;
    GLFWmonitor** monitors_ptr = glfwGetMonitors(&monitors_count);
    std::vector<monitor> ret;
    ret.reserve(monitors_count);
    for (int i=0; i<monitors_count; i++) ret.emplace_back(monitors_ptr[i]);
    return ret;
  }

  static monitor get_primary_monitor() {
    GLFWmonitor* monitors_ptr = glfwGetPrimaryMonitor();
    if (monitors_ptr == NULL) {
      glfw_error error = glfw_error::make_current_error();
      if(error.error_code == GLFW_NO_ERROR) return monitor{nullptr};
      throw error;
    }
    return monitor{monitors_ptr};
  }

  template <typename F>
  static void set_callback(F f) {
    _monitor_callback = f;
    if constexpr (std::same_as<F, nullptr_t>) {
      glfwSetMonitorCallback(NULL);
    }
    else {
      glfwSetMonitorCallback(monitor::_set_callback);
    }
  }

  explicit monitor(std::nullptr_t) {}

  explicit monitor(GLFWmonitor* ptr) : _internal_ptr(ptr) {}

  explicit operator GLFWmonitor*() { return _internal_ptr; }

  private:
  inline static std::function<void(GLFWmonitor*, int)> _monitor_callback;

  GLFWmonitor* _internal_ptr = NULL;

  static void _set_callback(GLFWmonitor* monitor, int event) {
    _monitor_callback(monitor, event);
  }
};

TGFX_NAMESPACE_END
