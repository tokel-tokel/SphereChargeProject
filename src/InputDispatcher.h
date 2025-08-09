#pragma once
#include <string_view>

class InputDispatcher 
{
public:
    virtual void takeInput(std::string_view str) = 0;
    virtual ~InputDispatcher() = default;
};
