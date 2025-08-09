#include "ConsoleInput.h"

#include <iostream>

ConsoleInput::ConsoleInput() : thread([this] { run(); }) {}

void ConsoleInput::run()
{
    std::string line;
    while (shouldClose.load())
    {
        if (std::cin.rdbuf()->in_avail() > 0)
        {
            if (std::getline(std::cin, line))
            {
                std::lock_guard lock(dispMutex);
                if (dispatcher) dispatcher->takeInput(line);
            }
        }
    }
}

void ConsoleInput::setDispatcher(const std::shared_ptr<InputDispatcher>& dispatcher_)
{
    std::lock_guard lock(dispMutex);
    this->dispatcher = dispatcher_;
}

ConsoleInput& ConsoleInput::getInstance()
{
    static ConsoleInput instance;
    return instance;
}

ConsoleInput::~ConsoleInput()
{
    shouldClose.store(true);
}


