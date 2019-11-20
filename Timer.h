#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
//declare this Timer object inside a scope to get the duration inside that scope
class Timer
{
    std::chrono::time_point<std::chrono::steady_clock> t;
public:
    Timer():t(std::chrono::steady_clock::now()){}
    ~Timer()
    {
        std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-t).count()<<"ms\n";
    }
};

#endif // TIMER_H
