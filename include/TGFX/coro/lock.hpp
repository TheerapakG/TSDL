#ifndef _TGFX_LOCK
#define _TGFX_LOCK

#include "TGFX/coro/eventloop.hpp"

#include <mutex>

TGFX_SUBSPACE_BEGIN(coro)

struct async_lock {
  deferred_task<> lock() {
    using namespace std::chrono_literals;
    while (true) {
      if (!_locked) {
        _locked = true;
        co_return;
      }
      co_await sleep(0ms);
    }
  }

  bool try_lock() {
    bool _curr_lock = _locked;
    _locked = true;
    return !_curr_lock;
  }

  template <typename Rep, typename Period>
  deferred_task<bool> try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration) {
    co_return co_await try_lock_until(update_now() + timeout_duration);
  }

  template <typename Clock, typename Duration>
  deferred_task<bool> try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
    if (try_lock()) co_return true;

    while (true) {
      if constexpr (std::same_as<Clock, std::chrono::steady_clock>) {
        auto time_left = timeout_time - update_now();
        if (time_left < Duration::zero()) co_return false;
        co_await sleep_until(std::min(timeout_time, ++get_running_loop_next_resume_time()));
      }
      else {
        auto time_left = timeout_time - Clock::now();
        auto steady_now = update_now();
        if (time_left < Duration::zero()) co_return false;
        co_await sleep_until(std::min(time_left+steady_now, ++get_running_loop_next_resume_time()));
      }
      if (try_lock()) co_return true;
    }
    co_return false;
  }

  void unlock() {
    _locked = false;
  }

  private:
  bool _locked;
};

template <typename underlying_mutex>
struct async_mutex {
  deferred_task<> lock() {
    using namespace std::chrono_literals;
    while(true) {
      if (!has_running_loop() || get_running_loop().empty()) {
        _mutex.lock();
        co_return;
      }
      else {
        if (_mutex.try_lock()) co_return;
        co_await sleep(0ms);
      }
    }
  }

  bool try_lock() { return _mutex.try_lock(); }
  
  template <typename Rep, typename Period>
  deferred_task<bool> try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration) requires requires {
    _mutex.try_lock_for(timeout_duration);
  } {
    std::chrono::steady_clock::time_point timeout_time = update_now() + timeout_duration;

    if (try_lock()) return true;

    while (true) {
      if (update_now() > timeout_time) co_return false;

      if (has_running_loop() && !get_running_loop().empty()) co_await sleep_until(std::min(timeout_time, ++get_running_loop_next_resume_time()));
      else if (_mutex.try_lock_until(timeout_time)) co_return true;

      if (try_lock()) co_return true;
    }
    co_return false;
  }

  template <typename Clock, typename Duration>
  deferred_task<bool> try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time) requires requires {
    _mutex.try_lock_until(timeout_time);
  } {
    if (try_lock()) co_return true;

    while (true) {
      if constexpr (std::same_as<Clock, std::chrono::steady_clock>) {
        auto time_left = timeout_time - update_now();
        if (time_left < Duration::zero()) co_return false;

        if (has_running_loop() && !get_running_loop().empty()) co_await sleep_until(std::min(timeout_time, ++get_running_loop_next_resume_time()));
        else if (_mutex.try_lock_until(timeout_time)) co_return true;
      }
      else {
        auto time_left = timeout_time - Clock::now();
        auto steady_now = update_now();
        if (time_left < Duration::zero()) co_return false;

        if (has_running_loop() && !get_running_loop().empty()) co_await sleep_until(std::min(time_left+steady_now, ++get_running_loop_next_resume_time()));
        else if (_mutex.try_lock_until(timeout_time)) co_return true;
      }
      if (try_lock()) co_return true;
    }
    co_return false;
  }

  void unlock() { _mutex.unlock(); }

  private:
  underlying_mutex _mutex;
};

template <typename Lock>
struct unique_lock {
  using lock_type = Lock;

  unique_lock() noexcept = default;
  unique_lock(unique_lock const&) = delete;
  unique_lock(unique_lock&& other) noexcept: 
    _lock(std::exchange(other._lock, nullptr)), 
    _owned(std::exchange(other._owned, false))
  {}
  unique_lock(lock_type& lock, std::defer_lock_t) noexcept:
    _lock(std::addressof(lock)) 
  {}
  unique_lock(lock_type& lock, std::try_to_lock_t):
    _lock(std::addressof(lock)), _owned(lock.try_lock()) 
  {}
  unique_lock(mutex_type& lock, std::adopt_lock_t):
    _lock(std::addressof(lock)), _owned(true) 
  {}
  template <typename Rep, typename Period>
  unique_lock(mutex_type& lock, const std::chrono::duration<Rep, Period>& timeout_duration):
    _lock(std::addressof(lock)), _owned(lock.try_lock_for(timeout_duration)) 
  {}
  template <typename Clock, typename Duration>
  unique_lock(mutex_type& lock, const std::chrono::time_point<Clock, Duration>& timeout_time):
    _lock(std::addressof(lock)), _owned(lock.try_lock_until(timeout_time)) 
  {}

  ~unique_lock() {
    if (_owned) _lock->unlock();
  }

  unique_lock& operator=(unique_lock&& other) noexcept {
    if (_owned) _lock->unlock();
    _lock = std::exchange(other._lock, nullptr);
    _owned = std::exchange(other._owned, false);
    return *this;
  }

  deferred_task<> lock() {
    if (_lock == nullptr) throw std::system_error(std::errc::operation_not_permitted, "tgfx::coro::unique_lock::lock: references null lock");
    if (_owned) throw std::system_error(std::errc::resource_deadlock_would_occur, "tgfx::coro::unique_lock::lock: already locked");
    co_await _lock->lock();
    _owned = true;
  }

  bool try_lock() {
    if (_lock == nullptr) throw std::system_error(std::errc::operation_not_permitted, "tgfx::coro::unique_lock::try_lock: references null lock");
    if (_owned) throw std::system_error(std::errc::resource_deadlock_would_occur, "tgfx::coro::unique_lock::try_lock: already locked");
    _owned = _lock->try_lock();
    return _owned;
  }

  template <typename Rep, typename Period>
  deferred_task<bool> try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration) {
    if (_lock == nullptr) throw std::system_error(std::errc::operation_not_permitted, "tgfx::coro::unique_lock::try_lock_for: references null lock");
    if (_owned) throw std::system_error(std::errc::resource_deadlock_would_occur, "tgfx::coro::unique_lock::try_lock_for: already locked");
    _owned = co_await _lock->try_lock_for(timeout_duration);
    co_return _owned;
  }

  template <typename Clock, typename Duration>
  deferred_task<bool> try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
    if (_lock == nullptr) throw std::system_error(std::errc::operation_not_permitted, "tgfx::coro::unique_lock::try_lock_until: references null lock");
    if (_owned) throw std::system_error(std::errc::resource_deadlock_would_occur, "tgfx::coro::unique_lock::try_lock_until: already locked");
    _owned = co_await _lock->try_lock_until(timeout_time);
    co_return _owned;
  }

  void unlock() {
    if (!_owned) throw std::system_error(std::errc::operation_not_permitted, "tgfx::coro::unique_lock::unlock: not locked");
    _lock->unlock();
    _owned = false;
  }

  void swap(unique_lock& other) noexcept {
    std::swap(_lock, other._lock);
    std::swap(_owned, other._owned);
  }

  lock_type* release() noexcept {
    _owned = false;
    return std::exchange(_lock, nullptr);
  }

  lock_type* underlying_lock() const noexcept { return _lock; }
  bool owns_lock() const noexcept { return _owned; }
  explicit operator bool() const noexcept { return _owned; }

  private:
  lock_type* _lock = nullptr;
  bool _owned = false;
};

template <typename Lock>
inline void swap(unique_lock<Lock>& x, unique_lock<Lock>& y) noexcept { x.swap(y); }

template <typename Lock>
deferred_task<unique_lock<Lock>> make_locked_unique_lock(Lock& lock) noexcept {
  co_await lock.lock();
  co_return {lock, std::adopt_lock};
}

template <typename L0, typename L1>
int try_lock(L0& l0, L1& l1) {
  if (l0.try_lock()) {
    if (l1.try_lock()) {
      return -1;
    }
    else return 1;
  }
  return 0;
}

template <typename L0, typename L1, typename L2, typename... L3>
int try_lock(L0& l0, L1& l1, L2& l2, L3&... l3) {
  if (l0.try_lock())
  {
    int i = try_lock(__l1, __l2, __l3...);
    if (i == -1) return -1;
    else return i + 1;
  }
  return 0;
}

template <typename L0, typename L1>
deferred_task<> lock(L0& l0, L0& l1) {
  while (true) {
    {
      auto u0 = co_await make_locked_unique_lock(l0);
      if (l1.try_lock()) {
        u0.release();
        break;
      }
    }
    co_await sleep(0);
    {
      auto u1 = co_await make_locked_unique_lock(l0);
      if (l0.try_lock()) {
        u1.release();
        break;
      }
    }
    co_await sleep(0);
  }
}

TGFX_SUBSPACE_END

TGFX_SUBSPACE_IMPL_BEGIN(coro)

template <typename L0, typename L1, typename L2, typename... L3>
deferred_task<> _lock(int i, L0& l0, L1& l1, L2& l2, L3&... l3) {
  while (true) {
    switch (i) {
      case 0:
        {
          auto u0 = co_await make_locked_unique_lock(l0);
          i = try_lock(l1, l2, l3...);
          if (i == -1) {
            u0.release();
            co_return;
          }
        }
        i++;
        co_await sleep(0);
        break;
      case 1:
        {
          auto u1 = co_await make_locked_unique_lock(l1);
          i = try_lock(l2, l3..., l0);
          if (i == -1) {
            u1.release();
            co_return;
          }
        }
        if (i == sizeof...(L3) + 1) i = 0;
        else i += 2;
        co_await sleep(0);
        break;
      default:
        co_await _lock(i - 2, l2, l3..., l0, l1);
        co_return;
    }
  }
}

template <typename... Locks>
struct [[nodiscard("tgfx::coro::impl::_scope_lock_coro_construct must be used to move-construct tgfx::coro::scoped_lock")]] 
_scope_lock_coro_construct {
  _scope_lock_coro_construct(Locks&... locks) : _t(locks...) {}
  _scope_lock_coro_construct(const _scope_lock_coro_construct&) = delete;
  _scope_lock_coro_construct(_scope_lock_coro_construct&&) = default;

  _scope_lock_coro_construct& operator=(const _scope_lock_coro_construct&) = delete;
  _scope_lock_coro_construct& operator=(_scope_lock_coro_construct&&) = delete;

  private:
  using LockTuple = std::tuple<Locks&...>;
  LockTuple _t;

  template <typename... _Locks>
  friend class scoped_lock;
};

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

template <typename L0, typename L1, typename L2, typename... L3>
deferred_task<> lock(int i, L0& l0, L1& l1, L2& l2, L3&... l3) {
  impl::_lock(0, l0, l1, l2, l3...);
}

template <typename... Locks>
struct scoped_lock {
  scoped_lock(std::adopt_lock_t, Locks&... locks) : _t(locks...) {}
  scoped_lock(_scope_lock_coro_construct<Locks...>&& scope_lock_construct) : _t(std::move(scope_lock_construct._t)) {}
  scoped_lock(const scoped_lock&) = delete;
  scoped_lock(scoped_lock&&) = delete;

  ~scoped_lock() {
    _unlock(std::index_sequence_for<Locks...>{});
  }

  scoped_lock& operator=(const scoped_lock&) = delete;
  scoped_lock& operator=(scoped_lock&&) = delete;

  private:
  using LockTuple = std::tuple<Locks&...>;
  LockTuple _t;

  template <size_t... Is>
  void _unlock(std::index_sequence<Is...>) {
    std::get<Is>(_t).unlock()...;
  }
};

template <typename... Locks>
deferred_task<impl::_scope_lock_coro_construct<Locks...>> make_locked_scope_lock(Locks&... locks) noexcept {
  co_await lock(locks...);
  co_return {locks...};
}

TGFX_SUBSPACE_END

#endif
