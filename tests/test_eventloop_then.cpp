#include "test_eventloop_common.hpp"

template <typename T>
immediate_task<> cout_value_task(T&& value) {
  std::cout << "cout value task " << value << std::endl;
  co_return;
}

template <typename T>
immediate_task<> cout_awaiter_task(typename deferred_task<T>::awaiter_type&& awaiter) {
  std::cout << "cout awaiter task " << awaiter.result() << std::endl;
  co_return;
}

template <typename T>
void cout_value_function(T&& value) {
  std::cout << "cout value function " << value << std::endl;
  return;
}

template <typename T>
void cout_awaiter_function(typename deferred_task<T>::awaiter_type&& awaiter) {
  std::cout << "cout awaiter function " << awaiter.result() << std::endl;
  return;
}

void create_then_tasks(bool print_pending = false) {
  std::cout << "creating then tasks" << std::endl;
  for(size_t i : std::views::iota(0ul, v.size())) {
    if(print_pending) std::cout << "pending tasks before created: " << loop.pending_tasks_count() << std::endl;
    loop.create_task(get_vector_index(v, i).then(cout_value_task<int>));
    loop.create_task(get_vector_index(v, i).then(cout_value_function<int>));
    loop.create_task(get_vector_index(v, i).then(cout_awaiter_task<int>));
    loop.create_task(get_vector_index(v, i).then(cout_awaiter_function<int>));
    loop.create_task(get_vector_index(v, i).then([](int x) -> immediate_task<> { std::cout << "cout value lambda task " << x << std::endl; co_return; }));
    loop.create_task(get_vector_index(v, i).then([](int x){ std::cout << "cout value lambda function " << x << std::endl; }));
    if(print_pending) std::cout << "pending tasks after created: " << loop.pending_tasks_count() << std::endl;
  }
  std::cout << "created then tasks" << std::endl;
}

int main() {
  spdlog::set_level(spdlog::level::debug);
  tgfx::coro::enable_task_debug();
  tgfx::coro::enable_task_src_stacktrace();
  tgfx::coro::enable_task_exec_stacktrace();
  create_then_tasks(true);
  initialize_vector();
  run_eventloop();
}
