#pragma once
#include <atomic>

#include "InputDispatcher.h"

class ParrotDispatcher : public InputDispatcher
{
private:
    std::atomic<bool>& exitFlag;
public:
    explicit ParrotDispatcher(std::atomic<bool>& exitFlag);
    void takeInput(std::string_view str) override;
};
