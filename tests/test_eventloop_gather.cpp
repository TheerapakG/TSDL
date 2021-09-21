#include "test_eventloop_common.hpp"

template<std::size_t... Is>
auto create_gather_task(std::index_sequence<Is...>) {
  return gather(
    get_vector_index(v, Is).then([](int x) -> deferred_task<> { std::cout << "cout value " << x << std::endl; co_return; })...
  );
}

deferred_task<> async_main() {
  initialize_vector();
  std::cout << "in async main" << std::endl;
  co_await create_gather_task(std::make_index_sequence<v_size>{});
  std::cout << "finished async main" << std::endl;
}

int main() {
  spdlog::set_level(spdlog::level::debug);
  tgfx::coro::enable_task_debug();
  tgfx::coro::enable_task_src_stacktrace();
  tgfx::coro::enable_task_exec_stacktrace();
  run(async_main());
}
