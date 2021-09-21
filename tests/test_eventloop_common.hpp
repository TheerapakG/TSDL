#ifndef _TGFX_TEST_EVENTLOOP_COMMON
#define _TGFX_TEST_EVENTLOOP_COMMON

#include <concepts>
#include <coroutine>
#include <iostream>
#include <numeric>
#include <deque>
#include <ranges>
#include <vector>

#include "TGFX/coro/eventloop.hpp"

using namespace tgfx::coro;

constexpr std::size_t v_size = 1<<2;
std::vector<int> v(v_size);
std::deque<queued_task<deferred_task<int>>> tasks;
std::deque<immediate_task<>> cout_tasks;

eventloop loop;

template <typename T>
deferred_task<T> get_vector_index(const std::vector<T>& v, size_t i) {
  std::cout << "get index value " << v[i] << std::endl;
  co_return v[i];
}

template <typename Awaitable>
immediate_task<> cout_awaitable_task(Awaitable&& awaitable) {
  try {
    auto result = co_await std::forward<Awaitable>(awaitable) ;
    std::cout << "cout task " << result << std::endl;
  }
  catch (const std::exception& e) {
    std::cout << "cout task " << e.what() << std::endl;
  }
}

template <typename Awaitable>
void cout_awaitable_function(Awaitable&& awaitable) {
  try {
    auto result = awaitable.result();
    std::cout << "cout function " << result << std::endl;
  }
  catch (const std::exception& e) {
    std::cout << "cout function " <<  e.what() << std::endl;
  }
}

void create_normal_tasks(bool print_pending = false) {
  std::cout << "creating normal tasks" << std::endl;
  for(size_t i : std::views::iota(0ul, v.size())) {
    if(print_pending) std::cout << "pending tasks before created: " << loop.pending_tasks_count() << std::endl;
    tasks.push_back(loop.create_task(get_vector_index(v, i)));
    if(print_pending) std::cout << "pending tasks after created: " << loop.pending_tasks_count() << std::endl;
  }
  std::cout << "created normal tasks" << std::endl;
}

void run_eventloop(bool print_pending = false) {
  if(print_pending) std::cout << "pending tasks before running loop: " << loop.pending_tasks_count() << std::endl;
  std::cout << "running eventloop" << std::endl;
  loop.run();
  std::cout << "ran eventloop" << std::endl;
  if(print_pending) std::cout << "pending tasks after running loop: " << loop.pending_tasks_count() << std::endl;
}

void exhaust_tasks() {
  std::cout << "calling cout tasks" << std::endl;
  for(auto t : tasks) cout_tasks.push_back(cout_awaitable_task(t));
  std::cout << "calling cout functions" << std::endl;
  for(auto t : tasks) cout_awaitable_function(t);

  tasks.clear();
  // TODO: remove finished cout_tasks
}

void initialize_vector() {
  std::iota(v.begin(), v.end(), 0);
  std::cout << "initialized vector" << std::endl;
}

#endif
