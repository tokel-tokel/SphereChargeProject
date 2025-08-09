#pragma once
#include <string>

template <class E>
constexpr std::string enumToString(E el)
{
    return std::to_string(static_cast<int>(el));
}