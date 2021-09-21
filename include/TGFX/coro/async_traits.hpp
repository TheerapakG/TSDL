#ifndef _TGFX_ASYNC_TRAITS
#define _TGFX_ASYNC_TRAITS

#include "TGFX/macros.hpp"

#include <utility>

TGFX_SUBSPACE_IMPL_BEGIN(coro::traits)

// used to roughly figure out if promise have await_transform
// note that if it have but cannot transform we will just let it breaks in the user code
template <typename Promise, typename T>
concept PromiseAwaitTransformable = requires (Promise& p, T&& expr) {
  { p.await_transform(std::forward<T>(expr)) };
};

template <typename Awaitable>
concept AwaitableHaveCoAwaitMember = requires (Awaitable&& a) {
  { std::forward<Awaitable>(a).operator co_await() };
};

template <typename Awaitable>
concept AwaitableHaveCoAwaitNonMember = requires (Awaitable&& a) {
  { operator co_await(std::forward<Awaitable>(a)) };
};

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro::traits)

template <typename Promise, typename T>
decltype(auto) get_awaitable(Promise& p, T&& expr) 
{
  if constexpr(impl::PromiseAwaitTransformable<Promise, T>) {
    return p.await_transform(std::forward<T>(expr));
  }
  else return std::forward<T>(expr);
}

template <typename Awaitable>
decltype(auto) get_awaiter(Awaitable&& a)
{
  if constexpr(impl::AwaitableHaveCoAwaitMember<Awaitable>) {
    return std::forward<Awaitable>(a).operator co_await();
  }
  else if constexpr(impl::AwaitableHaveCoAwaitNonMember<Awaitable>) {
    return operator co_await(std::forward<Awaitable>(a));
  }
  else return std::forward<Awaitable>(a);
}

template <typename Awaiter>
using get_awaiter_result_t = decltype(std::declval<Awaiter>().await_resume());

template <typename Awaitable>
using get_awaitable_result_t = get_awaiter_result_t<decltype(get_awaiter(std::forward<Awaitable>(std::declval<Awaitable>())))>;

template <typename T>
concept NormallyAwaitable = requires (T&& t) {
  { get_awaiter(std::forward<T>(t)).await_ready() } -> std::same_as<bool>;
  { get_awaiter(std::forward<T>(t)).await_suspend(std::declval<std::coroutine_handle<>>()) }; // void, bool, std::coroutine_handle<>
  { get_awaiter(std::forward<T>(t)).await_resume() };
};

template <typename Promise, typename T>
using get_co_await_result_t = get_awaitable_result_t<decltype(get_awaitable(std::declval<Promise>(), std::declval<T>()))>;

TGFX_SUBSPACE_END

#endif
