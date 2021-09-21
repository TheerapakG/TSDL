#ifndef _TGFX_TASK
#define _TGFX_TASK

#include "TGFX/macros.hpp"
#include "TGFX/coro/async_traits.hpp"

#include <concepts>
#include <coroutine>
#include <variant>

#ifdef TGFX_ENABLE_SPDLOG
#include "spdlog/spdlog.h"
#endif

#ifdef TGFX_ENABLE_BOOST_STACKTRACE
#include "TGFX/boost_stacktrace_helpers.hpp"
#endif

#ifdef TGFX_ENABLE_SPDLOG
TGFX_SUBSPACE_IMPL_BEGIN(coro)

bool _enable_task_debug = false;

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

bool enable_task_debug(bool enable = true) {
  bool ret = impl::_enable_task_debug;
  impl::_enable_task_debug = enable;
  return ret;
}

TGFX_SUBSPACE_END
#endif

#ifdef TGFX_ENABLE_STACKTRACE
TGFX_SUBSPACE_IMPL_BEGIN(coro)

bool _enable_task_src_stacktrace = false;
bool _enable_task_exec_stacktrace = false;

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

bool enable_task_src_stacktrace(bool enable = true) {
  bool ret = impl::_enable_task_src_stacktrace;
  impl::_enable_task_src_stacktrace = enable;
  return ret;
}

bool enable_task_exec_stacktrace(bool enable = true) {
  bool ret = impl::_enable_task_exec_stacktrace;
  impl::_enable_task_exec_stacktrace = enable;
  return ret;
}

TGFX_SUBSPACE_END
#endif

TGFX_SUBSPACE_IMPL_BEGIN(coro)

struct _impl_constructor_t {};
constexpr _impl_constructor_t _impl_constructor{};

template <typename result_type>
struct _task_value {
  using type = std::variant<result_type, std::exception_ptr>;
};

template <>
struct _task_value<void> {
  using type = std::exception_ptr;
};

template <typename result_type>
using _task_value_t = typename _task_value<result_type>::type;

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

enum class task_status {
  pending,
  executing,
  exception,
  cancelled,
  result
};

TGFX_SUBSPACE_END

TGFX_SUBSPACE_IMPL_BEGIN(coro)

template <typename result_type>
struct task_data {
  task_status _status = task_status::pending;
  _task_value_t<result_type> _data;
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
  std::optional<boost::stacktrace::stacktrace> _src_stacktrace;
  std::optional<boost::stacktrace::stacktrace> _exec_stacktrace;
#endif
};

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

struct cancelled_error: public std::exception {
  virtual const char* what() const noexcept{
    return "task was cancelled";
  }
};

struct invalid_state_error: public std::exception {
  virtual const char* what() const noexcept{
    return "task was not executed";
  }
};

template <typename Awaitable, typename FinalType>
struct base_awaitable_promise_unspecialized {
  using awaitable_type = Awaitable;

  std::coroutine_handle<> continuation = std::noop_coroutine();

  Awaitable get_return_object() noexcept {
    return Awaitable{impl::_impl_constructor, std::coroutine_handle<FinalType>::from_promise(static_cast<FinalType&>(*this))};
  }

  struct final_awaiter {
    bool await_ready() noexcept { return false; }
    std::coroutine_handle<> await_suspend(std::coroutine_handle<FinalType> h) noexcept {
      return h.promise().continuation;
    }
    void await_resume() noexcept {}
  };

  final_awaiter final_suspend() noexcept {
    return {};
  }
};

template <typename BasePromise, typename T>
struct base_awaitable_promise_specialized: public BasePromise {
  using result_type = T;
  impl::task_data<T> _task_data;

  template <std::convertible_to<T> U>
  void return_value(U&& u) { 
    _task_data._data.template emplace<0>(std::forward<U>(u));
    _task_data._status = task_status::result;
  }
  void unhandled_exception() { 
    _task_data._data.template emplace<1>(std::current_exception()); 
    _task_data._status = task_status::exception;
  }
};

template <typename BasePromise>
struct base_awaitable_promise_specialized<BasePromise, void>: public BasePromise {
  using result_type = void;
  impl::task_data<void> _task_data;

  void return_void() noexcept {
    _task_data._status = task_status::result;
  }
  void unhandled_exception() noexcept { 
    _task_data._data = std::current_exception(); 
    _task_data._status = task_status::exception;
  }
};

template <typename Awaitable, typename T, typename FinalType>
using base_awaitable_promise = base_awaitable_promise_specialized<base_awaitable_promise_unspecialized<Awaitable, FinalType>, T>;

template <typename Awaitable, typename T>
struct deferred_awaitable_promise: public base_awaitable_promise<Awaitable, T, deferred_awaitable_promise<Awaitable, T>> {
  std::suspend_always initial_suspend() noexcept { return {}; }
};

template <typename Awaitable, typename T>
struct immediate_awaitable_promise: public base_awaitable_promise<Awaitable, T, immediate_awaitable_promise<Awaitable, T>> {
  using base_type = base_awaitable_promise<Awaitable, T, immediate_awaitable_promise<Awaitable, T>>;
  std::suspend_never initial_suspend() noexcept { 
    // immediate task immediately starts executing
    base_type::_task_data._status = task_status::executing;
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
    if (impl::_enable_task_exec_stacktrace) base_type::_task_data._exec_stacktrace = boost::stacktrace::stacktrace{};
#endif
    return {}; 
  }
};

template <typename Promise, typename FinalType>
struct base_awaiter {
  using promise_type = Promise;

  std::coroutine_handle<> await_suspend(std::coroutine_handle<> continuation) noexcept {
    handle.promise().continuation = continuation;
    return handle;
  }

  auto result() const {
    return static_cast<const FinalType&>(*this).await_resume();
  }

  base_awaiter(base_awaiter&& aw): handle{std::exchange(aw.handle, {})} {}
  
  base_awaiter(impl::_impl_constructor_t, std::coroutine_handle<Promise> h) noexcept
  : handle{h} {}

  protected:
  std::coroutine_handle<Promise> handle;
};

template <typename Promise, typename FinalType>
struct non_owning_awaiter_unspecialized: public base_awaiter<Promise, FinalType> {
  using base_type = base_awaiter<Promise, FinalType>;
  using base_type::base_type;

  bool await_ready() noexcept {
    return base_type::handle.done();
  }
};

template <typename Promise, typename FinalType>
struct raii_awaiter_unspecialized: public base_awaiter<Promise, FinalType> {
  using base_type = base_awaiter<Promise, FinalType>;
  using base_type::base_type;

  ~raii_awaiter_unspecialized() {
    if (base_type::handle) {
#ifdef TGFX_ENABLE_SPDLOG
      if (impl::_enable_task_debug && base_type::handle.promise()._task_data._status == task_status::executing) {
        std::string dbg_str{"Task was destroyed but it is executing!"};
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
        boost::stacktrace::stacktrace stacktrace;
        auto dbg_str_inserter = std::back_inserter(dbg_str);

        boost_helpers::impl::format_stacktrace_to(dbg_str_inserter, "\nTraceback (most recent call last):", stacktrace, std::ranges::views::drop(1));
        if(base_type::handle.promise()._task_data._src_stacktrace) {
          boost_helpers::impl::format_stacktrace_to(
            dbg_str_inserter, 
            "\n(note) task was instantiated via the following traceback:",
            base_type::handle.promise()._task_data._src_stacktrace.value()
          );
        }
        if(base_type::handle.promise()._task_data._exec_stacktrace) {
          boost_helpers::impl::format_stacktrace_to(
            dbg_str_inserter, 
            "\n(note) task was initiated via the following traceback:", 
            base_type::handle.promise()._task_data._exec_stacktrace.value()
          );
        }
#endif
        spdlog::debug(dbg_str);
      }
#endif
      base_type::handle.destroy();
    }
  }
  
  bool await_ready() noexcept {
    base_type::handle.promise()._task_data._status = task_status::executing;
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
    if (impl::_enable_task_exec_stacktrace) base_type::handle.promise()._task_data._exec_stacktrace = boost::stacktrace::stacktrace{};
#endif
    return base_type::handle.done();
  }
};

template <typename UnspecializedAwaiter>
struct awaiter_specialized: public UnspecializedAwaiter {
  using base_type = UnspecializedAwaiter;
  using base_type::base_type;
  auto await_resume() const { 
    auto& _task_data = base_type::handle.promise()._task_data;
    // impl note: no canceling this task type
    if (_task_data._status == task_status::result) return std::get<0>(_task_data._data);
    else if (_task_data._status == task_status::exception) std::rethrow_exception(std::get<1>(_task_data._data));
    throw invalid_state_error{};
  }
};

template <typename UnspecializedAwaiter> requires std::same_as<typename UnspecializedAwaiter::promise_type::result_type, void>
struct awaiter_specialized<UnspecializedAwaiter>: public UnspecializedAwaiter {
  using base_type = UnspecializedAwaiter;
  using base_type::base_type;
  void await_resume() const { 
    auto& _task_data = base_type::handle.promise()._task_data;
    // impl note: no canceling this task type
    if (_task_data._status == task_status::result) return;
    else if (_task_data._status == task_status::exception) std::rethrow_exception(_task_data._data);
    throw invalid_state_error{};
  }
};

template <typename Promise>
struct raii_awaiter: public awaiter_specialized<raii_awaiter_unspecialized<Promise, raii_awaiter<Promise>>> {
  using base_type = awaiter_specialized<raii_awaiter_unspecialized<Promise, raii_awaiter<Promise>>>;
  using base_type::base_type;
};

template <typename Promise>
struct non_owning_awaiter: public awaiter_specialized<non_owning_awaiter_unspecialized<Promise, non_owning_awaiter<Promise>>> {
  using base_type = awaiter_specialized<non_owning_awaiter_unspecialized<Promise, non_owning_awaiter<Promise>>>;
  using base_type::base_type;
};

template <typename Promise>
struct raii_task {
  using promise_type = Promise;
  using awaiter_type = raii_awaiter<promise_type>;
  using awaitable_type = typename promise_type::awaitable_type;

  raii_task(raii_task&& t) noexcept: handle{std::exchange(t.handle, {})} {}

  ~raii_task() {
    if (handle) {
#ifdef TGFX_ENABLE_SPDLOG
      if (impl::_enable_task_debug && handle.promise()._task_data._status == task_status::executing) {
        std::string dbg_str{"Task was destroyed but it is executing!"};
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
        boost::stacktrace::stacktrace stacktrace;
        auto dbg_str_inserter = std::back_inserter(dbg_str);

        boost_helpers::impl::format_stacktrace_to(dbg_str_inserter, "\nTraceback (most recent call last):", stacktrace, std::ranges::views::drop(1));
        if(handle.promise()._task_data._src_stacktrace) {
          boost_helpers::impl::format_stacktrace_to(
            dbg_str_inserter, 
            "\n(note) task was instantiated via the following traceback:",
            handle.promise()._task_data._src_stacktrace.value()
          );
        }
        if(handle.promise()._task_data._exec_stacktrace) {
          boost_helpers::impl::format_stacktrace_to(
            dbg_str_inserter, 
            "\n(note) task was initiated via the following traceback:", 
            handle.promise()._task_data._exec_stacktrace.value()
          );
        }
#endif
        spdlog::debug(dbg_str);
      }
#endif
      handle.destroy();
    }
  }

  awaitable_type& operator=(awaitable_type&& t) noexcept {
    handle = std::exchange(t.handle, {});
    return static_cast<awaitable_type&>(*this);
  }

  awaiter_type operator co_await() && noexcept {
    return awaiter_type{impl::_impl_constructor, std::exchange(handle, {})};
  }


  std::coroutine_handle<promise_type> handle;

  explicit raii_task(impl::_impl_constructor_t, std::coroutine_handle<promise_type> h) noexcept: handle{h} {
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
    if (impl::_enable_task_src_stacktrace) h.promise()._task_data._src_stacktrace = boost::stacktrace::stacktrace{};
#endif
  }

  template <typename Awaitable_, typename FinalType_>
  friend struct base_awaitable_promise_unspecialized;
};

template <typename Promise>
struct non_owning_task {
  using promise_type = Promise;
  using awaiter_type = non_owning_awaiter<promise_type>;
  using awaitable_type = typename promise_type::awaitable_type;

  non_owning_task(non_owning_task&& t) noexcept: handle{std::exchange(t.handle, {})} {}

  ~non_owning_task() {
    if (handle) {
#ifdef TGFX_ENABLE_SPDLOG
      if (impl::_enable_task_debug && handle.promise()._task_data._status == task_status::pending) {
        std::string dbg_str{"Non-owning task was destroyed but it is pending!"};
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
        boost::stacktrace::stacktrace stacktrace;
        auto dbg_str_inserter = std::back_inserter(dbg_str);

        boost_helpers::impl::format_stacktrace_to(dbg_str_inserter, "\nTraceback (most recent call last):", stacktrace, std::ranges::views::drop(1));
        if(handle.promise()._task_data._src_stacktrace) {
          boost_helpers::impl::format_stacktrace_to(
            dbg_str_inserter, 
            "\n(note) task was instantiated via the following traceback:",
            handle.promise()._task_data._src_stacktrace.value()
          );
        }
#endif
        spdlog::debug(dbg_str);
      }
#endif
    }
  }

  awaitable_type& operator=(awaitable_type&& t) noexcept {
    handle = std::exchange(t.handle, {});
    return static_cast<awaitable_type&>(*this);
  }

  awaiter_type operator co_await() && noexcept {
    return awaiter_type{impl::_impl_constructor, std::exchange(handle, {})};
  }

  private:
  std::coroutine_handle<promise_type> handle;

  explicit non_owning_task(impl::_impl_constructor_t, std::coroutine_handle<promise_type> h) noexcept: handle{h} {
#ifdef TGFX_ENABLE_BOOST_STACKTRACE
    if (impl::_enable_task_src_stacktrace) h.promise()._task_data._src_stacktrace = boost::stacktrace::stacktrace{};
#endif
  }

  template <typename Awaitable_, typename FinalType_>
  friend struct base_awaitable_promise_unspecialized;
};

TGFX_SUBSPACE_END

TGFX_SUBSPACE_IMPL_BEGIN(coro)

template <typename To, typename From>
To _task_cast(From from) {
  co_return co_await from;
}

template <typename TaskBase, template <typename> typename TaskTemplate, typename T>
struct then_mixin: public TaskBase {
  using awaiter_type = typename TaskBase::awaiter_type;
  
  using TaskBase::TaskBase;

  template <typename F>
  auto then(F&& f) && {
    return then_mixin::_then(std::forward<F>(f), std::move(static_cast<TaskTemplate<T>&>(*this)));
  }

  private:
  template <typename F>
  static TaskTemplate<
    traits::get_awaitable_result_t<
      std::invoke_result_t<F, T>
    >
  > _then(F f, TaskTemplate<T> _this) requires traits::NormallyAwaitable<std::invoke_result_t<F, T>> {
    co_return co_await f(co_await std::move(_this));
  } 

  template <typename F>
  static TaskTemplate<
    traits::get_awaitable_result_t<
      std::invoke_result_t<F, awaiter_type>
    >
  > _then(F f, TaskTemplate<T> _this) requires traits::NormallyAwaitable<std::invoke_result_t<F, awaiter_type>> {
    auto awaiter = std::move(_this).operator co_await();
    co_await awaiter;
    co_return co_await f(std::move(awaiter));
  } 

  template <typename F>
  static TaskTemplate<
    std::invoke_result_t<F, T>
  > _then(F f, TaskTemplate<T> _this) {
    co_return f(co_await std::move(_this));
  } 

  template <typename F>
  static TaskTemplate<
    std::invoke_result_t<F, awaiter_type&&>
  > _then(F f, TaskTemplate<T> _this) {
    auto awaiter = std::move(_this).operator co_await();
    co_await awaiter;
    co_return f(std::move(awaiter));
  } 
};

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

template <typename To, typename From>
To task_cast(From&& from) requires requires {
  requires std::is_rvalue_reference_v<From>;
} {
  return _task_cast(std::move(from));
}

template <typename T=void>
struct deferred_task: public impl::then_mixin<raii_task<deferred_awaitable_promise<deferred_task<T>, T>>, deferred_task, T> {
  using base_type = impl::then_mixin<raii_task<deferred_awaitable_promise<deferred_task<T>, T>>, deferred_task, T>;
  using base_type::base_type;
};

template <typename T=void>
struct immediate_task: public impl::then_mixin<raii_task<immediate_awaitable_promise<immediate_task<T>, T>>, immediate_task, T> {
  using base_type = impl::then_mixin<raii_task<immediate_awaitable_promise<immediate_task<T>, T>>, immediate_task, T>;
  using base_type::base_type;
};

template <typename T=void>
struct deferred_non_owning_task: public impl::then_mixin<non_owning_task<deferred_awaitable_promise<deferred_non_owning_task<T>, T>>, deferred_non_owning_task, T> {
  using base_type = impl::then_mixin<non_owning_task<deferred_awaitable_promise<deferred_non_owning_task<T>, T>>, deferred_non_owning_task, T>;
  using base_type::base_type;
};

template <typename T=void>
struct immediate_non_owning_task: public impl::then_mixin<non_owning_task<immediate_awaitable_promise<immediate_non_owning_task<T>, T>>, immediate_non_owning_task, T> {
  using base_type = impl::then_mixin<non_owning_task<immediate_awaitable_promise<immediate_non_owning_task<T>, T>>, immediate_non_owning_task, T>;
  using base_type::base_type;
};

TGFX_SUBSPACE_END

#endif
