#include "test_eventloop_common.hpp"

template <typename T>
deferred_task<T> nested_get_vector_index(const std::vector<T>& v, size_t i) {
  std::cout << "nested get index value " << v[i] << std::endl;
  co_return co_await loop.create_task(get_vector_index(v, i));
}

void create_nested_tasks(bool print_pending = false) {
  std::cout << "creating nested tasks" << std::endl;
  for(size_t i : std::views::iota(0ul, v.size())) {
    if(print_pending) std::cout << "pending tasks before created: " << loop.pending_tasks_count() << std::endl;
    tasks.push_back(loop.create_task(nested_get_vector_index(v, i)));
    if(print_pending) std::cout << "pending tasks after created: " << loop.pending_tasks_count() << std::endl;
  }
  std::cout << "created nested tasks" << std::endl;
}

int main() {
  create_nested_tasks(true);
  initialize_vector();
  run_eventloop();
  exhaust_tasks();
}
