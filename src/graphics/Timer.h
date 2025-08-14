#pragma once
#include <chrono>

class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    float dtime;
public:
    Timer();
    void reset();
    void update();
    [[nodiscard]] float getDelta() const;
};