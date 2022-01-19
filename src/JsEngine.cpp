#include "JsEngine.h"
#include <chrono>
#include <thread>

Engine::Engine() : ctx(Context("")) {}

auto Engine::runString(std::string str) -> Value {
    Value val = ctx.eval(str);

    while (!tasks.empty()) {
        Task *task = tasks.front();
        tasks.pop();

        std::this_thread::sleep_for(std::chrono::milliseconds(task->delay));

        Value call_result = task->invoke();

        if (task->repeat) {
            tasks.push(task);
        }
    }

    return val;
}

auto Engine::getContext() -> Context { return ctx; }

void Engine::addTask(Task *task) { tasks.push(task); }
