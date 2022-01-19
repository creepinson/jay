#include "Task.h"

#include <utility>

Task::Task(Context *ctx, std::string func, Value *args, int delay, bool repeat)
    : ctx(ctx), func(std::move(func)), args(args), delay(delay), repeat(repeat) {}

auto Task::invoke() -> Value {
    Value val = ctx->callFunction(func);

    return val;
}
