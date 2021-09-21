#include "test_eventloop_common.hpp"

template <typename T>
deferred_non_owning_task<T> non_owning_get_vector_index(const std::vector<T>& v, size_t i) {
  std::cout << "non owning get index value " << v[i] << std::endl;
  co_return v[i];
}

void create_non_owning_tasks(bool print_pending = false) {
  std::cout << "creating non owning tasks" << std::endl;
  for(size_t i : std::views::iota(0ul, v.size())) {
    non_owning_get_vector_index(v, i);
  }
  std::cout << "created non owning tasks" << std::endl;
}

int main() {
  spdlog::set_level(spdlog::level::debug);
  tgfx::coro::enable_task_debug();
  tgfx::coro::enable_task_src_stacktrace();
  tgfx::coro::enable_task_exec_stacktrace();
  create_non_owning_tasks(true);
}
