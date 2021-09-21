#ifndef _TGFX_EVENTLOOP_FWD
#define _TGFX_EVENTLOOP_FWD

#include "TGFX/macros.hpp"

#include "TGFX/common_concepts.hpp"
#include "TGFX/coro/task.hpp"

#include <memory>

TGFX_SUBSPACE_IMPL_BEGIN(coro)

struct basic_task_data;

template <typename Awaitable>
struct queued_task_data;

struct scheduled_task;

struct schedule;

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

template <typename Awaitable>
class queued_task;

class eventloop;

bool has_running_loop();
eventloop& get_running_loop();

template <typename Awaitable>
void run(Awaitable&& aw);

template <typename Awaitable>
queued_task<Awaitable> create_task(Awaitable&& aw);

template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds, typename Awaitable>
queued_task<Awaitable> schedule_task(_milliseconds&& milliseconds, Awaitable&& aw);
template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point, typename Awaitable>
queued_task<Awaitable> schedule_task(_time_point&& timeout_time, Awaitable&& aw);

template <concepts::cvref_type_of<std::chrono::milliseconds> _milliseconds>
deferred_task<> sleep(_milliseconds&& milliseconds);

template <concepts::cvref_type_of<std::chrono::steady_clock::time_point> _time_point>
deferred_task<> sleep_until(_time_point&& timeout_time);

TGFX_SUBSPACE_END

TGFX_SUBSPACE_IMPL_BEGIN(coro)

template <typename... Awaitables>
struct gathered_task_data;

TGFX_SUBSPACE_IMPL_END

TGFX_SUBSPACE_BEGIN(coro)

template <typename... Awaitables>
class gathered_task;

template <typename... Awaitables>
gathered_task<Awaitables...> gather(Awaitables&&... aws);

void run_running_loop_once(bool allow_sleep = true);

template <typename Clock, typename Duration>
void run_running_loop_once_blocking_less_than(const std::chrono::time_point<Clock, Duration>& timeout_time);

std::chrono::steady_clock::time_point get_running_loop_next_resume_time();

TGFX_SUBSPACE_END

#endif
