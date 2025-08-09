#include "ParrotDispatcher.h"

#include <iostream>

ParrotDispatcher::ParrotDispatcher(std::atomic<bool>& exitFlag) : exitFlag(exitFlag) {}

void ParrotDispatcher::takeInput(std::string_view str)
{
    if (str == "exit") exitFlag.store(true);
    else std::cout << str << '\n';
}
