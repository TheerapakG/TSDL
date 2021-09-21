#include "test_eventloop_common.hpp"

template <typename T>
deferred_task<T> sleep_get_vector_index(const std::vector<T>& v, size_t i) {
  using namespace std::chrono_literals;
  std::time_t _time;
  _time = std::time(nullptr);
  std::cout << "before sleep get index value " << v[i] << ' ' << std::ctime(&_time) << std::endl;
  co_await sleep(500ms);
  _time = std::time(nullptr);
  std::cout << "after sleep get index value " << v[i] << ' ' << std::ctime(&_time) << std::endl;
  co_return v[i];
}

void create_sleep_tasks(bool print_pending = false) {
  std::cout << "creating sleep tasks" << std::endl;
  for(size_t i : std::views::iota(0ul, v.size())) {
    if(print_pending) std::cout << "pending tasks before created: " << loop.pending_tasks_count() << std::endl;
    tasks.push_back(loop.create_task(sleep_get_vector_index(v, i)));
    if(print_pending) std::cout << "pending tasks after created: " << loop.pending_tasks_count() << std::endl;
  }
  std::cout << "created sleep tasks" << std::endl;
}

int main() {
  create_sleep_tasks(true);
  initialize_vector();
  run_eventloop();
  exhaust_tasks();
}
