#ifndef _TGFX_EVENTLOOP_FWD_MEM
#define _TGFX_EVENTLOOP_FWD_MEM

#include "TGFX/coro/eventloop_fwd.hpp"

#include <chrono>
#include <queue>
#include <set>

TGFX_SUBSPACE_IMPL_BEGIN(coro)

thread_local std::optional<std::reference_wrapper<eventloop>> _current_eventloop;

struct basic_task_data {
  std::deque<std::coroutine_handle<>> _handles;
  task_status _status = task_status::pending;
};

template <typename Awaitable>
struct queued_task_data: public basic_task_data {
  using result_type = std::remove_cvref_t<traits::get_awaitable_result_t<Awaitable>>;

  std::remove_cvref_t<Awaitable> _awaitable;
  eventloop& _evloop;
  _task_value_t<result_type> _data;

  void cancel();

  template <concepts::cvref_type_of<Awaitable> _Awaitable>
  constexpr queued_task_data(_Awaitable&& aw, eventloop& evloop) noexcept(std::copy_constructible<Awaitable>): 
    _awaitable(std::forward<_Awaitable>(aw)), 
    _evloop{evloop} 
  {}
};

struct scheduled_task {
  struct promise_type {
    scheduled_task get_return_object() noexcept {
      return scheduled_task{impl::_impl_constructor, std::coroutine_handle<promise_type>::from_promise(static_cast<promise_type&>(*this))};
    }

    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }

    void return_void() noexcept {}
    void unhandled_exception() noexcept {}
  };

  scheduled_task(scheduled_task&& t) noexcept: handle{std::exchange(t.handle, {})} {}

  scheduled_task& operator=(scheduled_task&& t) noexcept {
    handle = std::exchange(t.handle, {});
    return static_cast<scheduled_task&>(*this);
  }

  private:
  std::coroutine_handle<promise_type> handle;

  explicit scheduled_task(impl::_impl_constructor_t, std::coroutine_handle<promise_type> h) noexcept: handle{h} {}

  friend struct promise_type;

  friend class ::tgfx::coro::eventloop;
};

struct schedule {
  std::chrono::steady_clock::time_point time;
  scheduled_task task;

  schedule(schedule&&) = default;

  template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
  schedule(_time_point&& time, scheduled_task&& task);

  schedule& operator=(schedule&&) = default;

  constexpr bool operator==(const schedule&) const;
  constexpr auto operator<=>(const schedule&) const;
};

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

template <typename Awaitable>
class queued_task {
  public:
  using result_type = traits::get_awaitable_result_t<Awaitable>;

  queued_task(const queued_task& t) = default;
  queued_task(queued_task&& t) = default;
  ~queued_task() = default;

  queued_task& operator=(const queued_task& t) = default;
  queued_task& operator=(queued_task&& t) = default;

  bool operator==(const queued_task& t) const {
    return _task_data == t._task_data;
  }
  auto operator<=>(const queued_task& t) const { 
    return _task_data <=> t._task_data; 
  }

  bool await_ready() const {
    task_status current_status = status();
    return current_status != task_status::pending && current_status != task_status::executing;
  }

  std::coroutine_handle<> await_suspend(std::coroutine_handle<> handle) const {
    // add handle that just got suspended to the handle to be awaken after we get result
    _task_data->_handles.push_back(std::move(handle));
    // do nothing, go back
    return std::noop_coroutine();
  }

  result_type await_resume() const {
    return result();
  }
  
  void cancel() {
    _task_data->cancel();
  }

  task_status status() const {
    return _task_data->_status;
  }

  result_type result() const {
    task_status current_status = status();
    if constexpr(std::same_as<result_type, void>) {
      if (current_status == task_status::result) return;
      else if (current_status == task_status::cancelled) throw cancelled_error{};
      else if (current_status == task_status::exception) std::rethrow_exception(_task_data->_data);
    }
    else {
      if (current_status == task_status::result) return std::get<0>(_task_data->_data);
      else if (current_status == task_status::cancelled) throw cancelled_error{};
      else if (current_status == task_status::exception) std::rethrow_exception(std::get<1>(_task_data->_data));
    }
    throw invalid_state_error{};
  }

  std::exception_ptr exception() const {
    task_status current_status = status();
    if constexpr(std::same_as<result_type, void>) {
      if (current_status == task_status::exception) std::rethrow_exception(_task_data->_data);
    }
    else {
      if (current_status == task_status::exception) std::rethrow_exception(std::get<1>(_task_data->_data));
    }
    if (current_status == task_status::cancelled) throw cancelled_error{};
    else if (current_status == task_status::result) return std::exception_ptr{};
    throw invalid_state_error{};
  }

  private:
  std::shared_ptr<impl::queued_task_data<Awaitable>> _task_data;

  queued_task(Awaitable&& awaitable, eventloop& evloop): 
    _task_data{std::make_shared<impl::queued_task_data<Awaitable>>(std::forward<Awaitable>(awaitable), evloop)}
  {}

  friend class eventloop;
};

class eventloop {
  public:
  template <typename Awaitable>
  queued_task<Awaitable> create_task(Awaitable&& aw);

  template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds, typename Awaitable>
  queued_task<Awaitable> schedule_task(_milliseconds&& milliseconds, Awaitable&& aw);
  template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point, typename Awaitable>
  queued_task<Awaitable> schedule_task(_time_point&& timeout_time, Awaitable&& aw);

  void run_handle_soon(std::coroutine_handle<>&& handle);
  template <typename Container>
  void run_handles_soon(Container& handle_container);

  template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds>
  deferred_task<> sleep(_milliseconds&& milliseconds);
  template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
  deferred_task<> sleep_until(_time_point&& timeout_time);

  void run();

  void run_once(bool allow_sleep = true);
  template <typename Clock, typename Duration>
  void run_once_blocking_less_than(const std::chrono::time_point<Clock, Duration>& timeout_time);

  void stop();

  size_t pending_tasks_count() const;
  bool empty() const;

  std::chrono::steady_clock::time_point now();
  std::chrono::steady_clock::time_point update_now();
  std::chrono::steady_clock::time_point get_next_resume_time();
  
  private:
  bool _running = false;
  std::vector<impl::schedule> _pending_tasks;
  std::queue<std::coroutine_handle<>> _pending_handles;
  std::set<const impl::basic_task_data*> _cancelled_tasks_data;
  std::chrono::steady_clock::time_point _now;

  template <typename Awaitable>
  impl::scheduled_task _execute_task_data(std::shared_ptr<impl::queued_task_data<Awaitable>>);

  template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
  void _add_task(_time_point&& time, impl::scheduled_task&& task);
  impl::scheduled_task _pop_task();
  void _clean_pending_handles();

  void _cancel(const impl::basic_task_data*);
  void _uncancel(const impl::basic_task_data*);

  template <typename Awaitable>
  friend class impl::queued_task_data;
};

TGFX_SUBSPACE_END

TGFX_SUBSPACE_IMPL_BEGIN(coro)

template <typename... Awaitables>
struct gathered_task_data: public basic_task_data {
  using result_type = std::tuple<queued_task<Awaitables>...>;
  
  std::variant<
    std::tuple<Awaitables...>,
    std::tuple<queued_task<Awaitables>...>
  > _awaitables;

  void cancel() {
    if (_status != task_status::pending || _status != task_status::executing) return;

    if(_status == task_status::executing) std::apply(
      [](const queued_task<Awaitables>&... awaitables){ ((awaitables.cancel(), void()), ...); },
      std::get<1>(std::move(_awaitables))
    );

    _status = task_status::cancelled;

    if (has_running_loop()) get_running_loop().run_handles_soon(_handles);
    else while (!_handles.empty()) {
      _handles.front().resume();
      _handles.pop_front();
    }
  }

  template <concepts::cvref_type_of<Awaitables>... _Awaitables>
  constexpr gathered_task_data(_Awaitables&&... aws): 
    _awaitables(std::in_place_index<0>, std::forward<_Awaitables>(aws)...) 
  {}
};

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

template <typename... Awaitables>
struct gathered_task {
  using result_type = std::tuple<queued_task<Awaitables>...>;

  gathered_task(const gathered_task&) = default;
  gathered_task(gathered_task&&) = default;
  ~gathered_task() = default;

  gathered_task& operator=(const gathered_task&) = default;
  gathered_task& operator=(gathered_task&&) = default;

  bool operator==(const gathered_task& t) const {
    return _gathered_task_data == t._gathered_task_data;
  }
  auto operator<=>(const gathered_task& t) const { 
    return _gathered_task_data <=> t._gathered_task_data; 
  }

  bool await_ready() const {
    task_status current_status = status();
    if(current_status == task_status::pending) {
      _gathered_task_data->_status = task_status::executing;

      auto& loop = get_running_loop();
      std::apply(
        [this, &loop](Awaitables&&... awaitables){
          _gathered_task_data->_awaitables.template emplace<1>(loop.create_task(std::move(awaitables))...);
        },
        std::get<0>(std::move(_gathered_task_data->_awaitables))
      );

      loop.create_task(
        std::apply(
          std::bind_front(
            [](
              std::shared_ptr<impl::gathered_task_data<Awaitables...>> _gathered_task_data, 
              const queued_task<std::remove_cvref_t<Awaitables>>&... queued_awaitables
            ) mutable -> deferred_task<> {
              ((co_await queued_awaitables, void()), ...);

              _gathered_task_data->_status = task_status::result;

              get_running_loop().run_handles_soon(_gathered_task_data->_handles);
            },
            _gathered_task_data
          ),
          std::get<1>(_gathered_task_data->_awaitables)
        )
      );
    }
    return current_status != task_status::pending && current_status != task_status::executing;
  }

  std::coroutine_handle<> await_suspend(std::coroutine_handle<> handle) {
    // add handle that just got suspended to the handle to be awaken after we get result
    _gathered_task_data->_handles.push_back(std::move(handle));
    // do nothing, go back
    return std::noop_coroutine();
  }

  result_type await_resume() const {
    return result();
  }
  
  void cancel() {
    _gathered_task_data->cancel();
  }

  task_status status() const {
    return _gathered_task_data->_status;
  }

  result_type result() const {
    task_status current_status = status();
    if (current_status == task_status::result) return std::get<1>(_gathered_task_data->_awaitables);
    else if (current_status == task_status::cancelled) throw cancelled_error{};
    throw invalid_state_error{};
  }

  std::exception_ptr exception() const {
    task_status current_status = status();
    if (current_status == task_status::cancelled) throw cancelled_error{};
    else if (current_status == task_status::result) return std::exception_ptr{};
    throw invalid_state_error{};
  }

  private:
  std::shared_ptr<impl::gathered_task_data<Awaitables...>> _gathered_task_data;

  template <concepts::cvref_type_of<Awaitables>... _Awaitables>
  gathered_task(_Awaitables&&... awaitables): 
    _gathered_task_data{std::make_shared<impl::gathered_task_data<Awaitables...>>(std::forward<_Awaitables>(awaitables)...)}
  {}

  template <typename... _Awaitables>
  friend gathered_task<_Awaitables...> gather(_Awaitables&&... aws);
};

template <typename... Awaitables>
gathered_task(Awaitables&&... awaitables) -> gathered_task<std::remove_cvref_t<Awaitables>...>;

TGFX_SUBSPACE_END

#endif
