#include "test_eventloop_common.hpp"

int main() {
  spdlog::set_level(spdlog::level::debug);
  tgfx::coro::enable_task_debug();
  tgfx::coro::enable_task_src_stacktrace();
  tgfx::coro::enable_task_exec_stacktrace();
  create_normal_tasks(true);
  initialize_vector();
  exhaust_tasks();
  cout_tasks.clear();
  run_eventloop();
}
