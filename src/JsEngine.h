#pragma once
#include "Core.h"
#include "Task.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>

class Engine {
    private:
        std::queue<Task *> tasks;
        Context ctx;

    public:
        Engine();
        auto runString(std::string str) -> Value;
        void addTask(Task *task);
        auto getContext() -> Context;
};
