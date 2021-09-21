#include "test_eventloop_common.hpp"

void create_cancelled_tasks(bool print_pending = false) {
  std::cout << "creating cancelled tasks" << std::endl;
  for(size_t i : std::views::iota(0ul, v.size())) {
    if(print_pending) std::cout << "pending tasks before created: " << loop.pending_tasks_count() << std::endl;
    auto task = loop.create_task(get_vector_index(v, i));
    if(print_pending) std::cout << "pending tasks after created / before cancelled: " << loop.pending_tasks_count() << std::endl;
    task.cancel();
    if(print_pending) std::cout << "pending tasks after cancelled: " << loop.pending_tasks_count() << std::endl;
    tasks.push_back(std::move(task));
  }
  std::cout << "created cancelled tasks" << std::endl;
}

int main() {
  create_cancelled_tasks(true);
  initialize_vector();
  run_eventloop();
  exhaust_tasks();
}
