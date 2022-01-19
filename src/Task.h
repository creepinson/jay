#pragma once
#include "Core.h"

class Task {
    public:
        Context *ctx;
        std::string func;
        Value *args;
        int delay;
        bool repeat;
        Task(Context *ctx, std::string func, Value *args, int delay,
             bool repeat);
        auto invoke() -> Value;
};
