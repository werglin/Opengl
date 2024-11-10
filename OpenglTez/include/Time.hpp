#pragma once
#ifndef TIME_hpp
#define TIME_hpp

#include<chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> HighResClock;

class Time {
public:
    Time();
    float ElapsedTimeInSeconds();
    float ApplicationElapsedInSeconds();

    static float s_deltaTime;
private:
    HighResClock _start;
    HighResClock _absStart;
    HighResClock _end;

};

#endif