#include "Timer.h"



using namespace projectn::utils;

void Timer::start()
{
    _startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    auto nowTime = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - _startTime).count();
    _duration = time / 1000.0;
}
