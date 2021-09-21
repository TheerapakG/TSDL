#include "test_eventloop_common.hpp"

template <typename T>
deferred_task<T> schedule_get_vector_index(const std::vector<T>& v, size_t i) {
  using namespace std::chrono_literals;
  std::cout << "before schedule get index value " << v[i] << std::endl;
  auto ret = co_await loop.schedule_task(5000ms, get_vector_index(v, i));
  std::cout << "after schedule get index value " << v[i] << std::endl;
  co_return ret;
}

void create_schedule_tasks(bool print_pending = false) {
  std::cout << "creating schedule tasks" << std::endl;
  for(size_t i : std::views::iota(0ul, v.size())) {
    if(print_pending) std::cout << "pending tasks before created: " << loop.pending_tasks_count() << std::endl;
    tasks.push_back(loop.create_task(schedule_get_vector_index(v, i)));
    if(print_pending) std::cout << "pending tasks after created: " << loop.pending_tasks_count() << std::endl;
  }
  std::cout << "created schedule tasks" << std::endl;
}

int main() {
  create_schedule_tasks(true);
  initialize_vector();
  run_eventloop();
  exhaust_tasks();
}
