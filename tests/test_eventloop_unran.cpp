#include "test_eventloop_common.hpp"

int main() {
  create_normal_tasks(true);
  initialize_vector();
  exhaust_tasks();
  run_eventloop();
}
