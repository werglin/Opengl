#include "Time.hpp"

float Time::s_deltaTime = 0.0f;

Time::Time()
{
    _start = std::chrono::high_resolution_clock::now();
    _absStart = _start;
}


float Time::ElapsedTimeInSeconds()
{
    _end = std::chrono::high_resolution_clock::now();
    auto difference = _end - _start;
    auto elapeseTime = std::chrono::duration<float>(difference);
    _start = _end;
    // saniye cinsinden zamaný döndürüyor
    return elapeseTime.count();
}

float Time::ApplicationElapsedInSeconds()
{
    _end = std::chrono::high_resolution_clock::now();
    auto difference = _end - _absStart;
    auto elapeseTime = std::chrono::duration<float>(difference);
    // saniye cinsinden zamaný döndürüyor
    return elapeseTime.count();
}
