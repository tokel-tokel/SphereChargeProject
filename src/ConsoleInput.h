#pragma once
#include <atomic>
#include <memory>
#include <thread>

#include "InputDispatcher.h"

class ConsoleInput 
{
private:
    std::atomic<bool> shouldClose{false};
    std::mutex dispMutex;
    std::shared_ptr<InputDispatcher> dispatcher{nullptr};
    std::thread thread;
public:
    ConsoleInput(const ConsoleInput&) = delete;
    ConsoleInput(ConsoleInput&&) = delete;
    ConsoleInput& operator=(const ConsoleInput&) = delete;
    ConsoleInput& operator=(ConsoleInput&&) = delete;
    void setDispatcher(const std::shared_ptr<InputDispatcher>& dispatcher_);
    void run();
    static ConsoleInput& getInstance();
    ~ConsoleInput();
private:
    ConsoleInput();
};
