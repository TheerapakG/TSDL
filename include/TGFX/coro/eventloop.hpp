#ifndef _TGFX_EVENTLOOP
#define _TGFX_EVENTLOOP

#include "TGFX/coro/eventloop_fwd_mem.hpp"

#include <coroutine>
#include <exception>
#include <functional>
#include <new>
#include <stdexcept>

TGFX_SUBSPACE_IMPL_BEGIN(coro)

template <typename Awaitable>
void queued_task_data<Awaitable>::cancel() {
  if (_status != task_status::pending) return;

  _status = task_status::cancelled;

  _evloop._cancel(this);

  _evloop.run_handles_soon(_handles);
}


template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
schedule::schedule(_time_point&& time, scheduled_task&& task) : time{std::forward<_time_point>(time)}, task{std::move(task)} {}

constexpr bool schedule::operator==(const schedule& other) const { 
  return time == other.time; 
}
constexpr auto schedule::operator<=>(const schedule& other) const { 
  return time <=> other.time; 
}

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

template <typename Awaitable>
queued_task<Awaitable> eventloop::create_task(Awaitable&& aw) {
  return schedule_task(std::chrono::steady_clock::time_point::min(), std::forward<Awaitable>(aw));
}

template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds, typename Awaitable>
queued_task<Awaitable> eventloop::schedule_task(_milliseconds&& milliseconds, Awaitable&& aw) {
  return schedule_task(update_now() + milliseconds, std::forward<Awaitable>(aw));
}

template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point, typename Awaitable>
queued_task<Awaitable> eventloop::schedule_task(_time_point&& timeout_time, Awaitable&& aw) {
  queued_task<Awaitable> task{std::forward<Awaitable>(aw), *this};
  _add_task(timeout_time, _execute_task_data(task._task_data));
  return task;
}

void eventloop::run_handle_soon(std::coroutine_handle<>&& handle) {
  _pending_handles.push(std::move(handle));
}

template <typename Container>
void eventloop::run_handles_soon(Container& handle_container) {
  auto begin = std::ranges::begin(handle_container);
  auto end = std::ranges::end(handle_container);
  for (; begin != end; begin++) {
    std::coroutine_handle<> handle_copy = *begin;
    run_handle_soon(std::move(handle_copy));
  }
  handle_container.clear();
}

template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds>
deferred_task<> eventloop::sleep(_milliseconds&& milliseconds) {
  if (milliseconds <= std::chrono::milliseconds::zero()) {
    if (get_next_resume_time() <= update_now()) co_await std::suspend_always{};
    co_return;
  }
  co_await schedule_task(std::forward<_milliseconds>(milliseconds), std::suspend_never{});
}

template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
deferred_task<> eventloop::sleep_until(_time_point&& timeout_time) {
  if (timeout_time <= update_now()) {
    if (get_next_resume_time() <= now()) co_await std::suspend_always{};
    co_return;
  }
  co_await schedule_task(std::forward<_time_point>(timeout_time), std::suspend_never{});
}

void eventloop::run() {
  if (impl::_current_eventloop) throw std::runtime_error("there is already a running loop");
  _running = true;
  impl::_current_eventloop = std::ref(*this);
  while (_running) {
    if(empty()) stop();
    run_once();
  }
}

void eventloop::run_once(bool allow_sleep) {
  if (allow_sleep) run_once_blocking_less_than(std::chrono::steady_clock::time_point::max());
  else run_once_blocking_less_than(std::chrono::steady_clock::time_point::min());
}

template <typename Clock, typename Duration>
void eventloop::run_once_blocking_less_than(const std::chrono::time_point<Clock, Duration>& timeout_time) {
  bool loop_is_running = _running;
  std::optional<std::reference_wrapper<eventloop>> running_loop;
  if (!loop_is_running) {
    _running = true;
    running_loop = std::exchange(impl::_current_eventloop, std::ref(*this));
  }
  auto time = get_next_resume_time();
  auto now = update_now();
  if (time <= now) {
    _pop_task().handle.resume();
    _clean_pending_handles();
  }
  else if (time < std::chrono::steady_clock::time_point::max()) {
    if constexpr (std::same_as<Clock, std::chrono::steady_clock>) {
      auto sleep_to = std::min(time, timeout_time);
      if (sleep_to > now) std::this_thread::sleep_until(sleep_to);
      else return;
    }
    else {
      auto timeout_time_this_clock = (timeout_time - Clock::now()) + now;
      auto sleep_to = std::min(time, timeout_time_this_clock);
      if (sleep_to > now) std::this_thread::sleep_until(sleep_to);
      else return;
    }
    run_once_blocking_less_than(timeout_time);
  }
  if (!loop_is_running) {
    impl::_current_eventloop = running_loop;
    _running = false;
  }
}

void eventloop::stop() {
  if (!_running) throw std::runtime_error("loop not running");
  impl::_current_eventloop.reset();
  _running = false;
}

size_t eventloop::pending_tasks_count() const {
  return _pending_tasks.size() - _cancelled_tasks_data.size();
}

bool eventloop::empty() const {
  return _pending_tasks.size() == _cancelled_tasks_data.size();
}

std::chrono::steady_clock::time_point eventloop::now() {
  if (_running) return _now;
  return std::chrono::steady_clock::now();
}

std::chrono::steady_clock::time_point eventloop::update_now() {
  _now = std::chrono::steady_clock::now();
  return _now;
}

std::chrono::steady_clock::time_point eventloop::get_next_resume_time() {
  if(!_pending_tasks.empty()) return _pending_tasks.begin()->time;
  return std::chrono::steady_clock::time_point::max();
}

template <typename Awaitable>
impl::scheduled_task eventloop::_execute_task_data(std::shared_ptr<impl::queued_task_data<Awaitable>> _task_data) {
  using result_type = traits::get_awaitable_result_t<Awaitable>;

  if (_task_data->_status == task_status::cancelled) {
    _uncancel(_task_data.get());
    co_return;
  }

  if (_task_data->_status != task_status::pending) co_return;

  _task_data->_status = task_status::executing;
  
  if constexpr(std::same_as<result_type, void>) {
    try {
      co_await std::move(_task_data->_awaitable);
      _task_data->_status = task_status::result;
    }
    catch(...) {
      _task_data->_data = std::current_exception();
      _task_data->_status = task_status::exception;
    }
  }
  else {
    try {
      _task_data->_data.template emplace<0>(co_await std::move(_task_data->_awaitable));
      _task_data->_status = task_status::result;
    }
    catch(...) {
      _task_data->_data.template emplace<1>(std::current_exception());
      _task_data->_status = task_status::exception;
    }
  }

  run_handles_soon(_task_data->_handles);
  co_return;
}

template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
void eventloop::_add_task(_time_point&& time, impl::scheduled_task&& task) {
  _pending_tasks.emplace_back(std::forward<_time_point>(time), std::move(task));
  std::push_heap(_pending_tasks.begin(), _pending_tasks.end(), std::greater<impl::schedule>{});
}

impl::scheduled_task eventloop::_pop_task() {
  impl::scheduled_task task = std::move(_pending_tasks.begin()->task);
  std::pop_heap(_pending_tasks.begin(), _pending_tasks.end(), std::greater<impl::schedule>{});
  _pending_tasks.pop_back();
  return task;
}

void eventloop::_clean_pending_handles() {
  while(!_pending_handles.empty()) {
    _pending_handles.front().resume();
    _pending_handles.pop();
  }
}

void eventloop::_cancel(const impl::basic_task_data* task_data_ptr) {
  _cancelled_tasks_data.insert(task_data_ptr);
}
void eventloop::_uncancel(const impl::basic_task_data* task_data_ptr) {
  _cancelled_tasks_data.erase(task_data_ptr);
}

eventloop& get_running_loop() {
  return impl::_current_eventloop.value().get();
}

bool has_running_loop() {
  return impl::_current_eventloop.has_value();
}

template <typename Awaitable>
void run(Awaitable&& aw) {
  eventloop loop;
  loop.create_task(std::forward<Awaitable>(aw));
  loop.run();
}

template <typename Awaitable>
queued_task<Awaitable> create_task(Awaitable&& aw) {
  if (has_running_loop()) return get_running_loop().create_task(std::forward<Awaitable>(aw));
  else throw std::runtime_error("no running event loop");
}

template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds, typename Awaitable>
queued_task<Awaitable> schedule_task(_milliseconds&& milliseconds, Awaitable&& aw) {
  if (has_running_loop()) return get_running_loop().schedule_task(std::forward<_milliseconds>(milliseconds), std::forward<Awaitable>(aw));
  else throw std::runtime_error("no running event loop");
}

template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point, typename Awaitable>
queued_task<Awaitable> schedule_task(_time_point&& timeout_time, Awaitable&& aw) {
  if (has_running_loop()) return get_running_loop().schedule_task(std::forward<_time_point>(timeout_time), std::forward<Awaitable>(aw));
  else throw std::runtime_error("no running event loop");
}

template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds>
deferred_task<> sleep(_milliseconds&& milliseconds) {
  if (has_running_loop()) co_await get_running_loop().sleep(std::forward<_milliseconds>(milliseconds));
  else if (milliseconds <= std::chrono::milliseconds::zero()) std::this_thread::yield();
  else std::this_thread::sleep_for(milliseconds);
  co_return;
}

template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
deferred_task<> sleep_until(_time_point&& timeout_time) {
  if(has_running_loop()) co_await get_running_loop().sleep_until(std::forward<_time_point>(timeout_time));
  else if (timeout_time <= std::chrono::steady_clock::now()) std::this_thread::yield();
  else std::this_thread::sleep_until(timeout_time);
  co_return;
}

template <typename... Awaitables>
gathered_task<Awaitables...> gather(Awaitables&&... aws) {
  return {std::forward<Awaitables>(aws)...};
}

void run_running_loop_once(bool allow_sleep) {
  if(has_running_loop()) get_running_loop().run_once(allow_sleep);
}

template <typename Clock, typename Duration>
void run_running_loop_once_blocking_less_than(const std::chrono::time_point<Clock, Duration>& timeout_time) {
  if(has_running_loop()) return get_running_loop().run_once_blocking_less_than(timeout_time);
  std::this_thread::sleep_for(timeout_time);
}

std::chrono::steady_clock::time_point get_running_loop_next_resume_time() {
  if(has_running_loop()) return get_running_loop().get_next_resume_time();
  else return std::chrono::steady_clock::time_point::max();
}

TGFX_SUBSPACE_END

#endif
