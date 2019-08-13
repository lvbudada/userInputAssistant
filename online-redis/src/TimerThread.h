#pragma once
#include "Timerfd.h"
#include "Thread.h"

namespace wd
{

class TimerThread
{
    using TimerCallback = function<void()>;
public:
    TimerThread(int initialTime, int intervalTime, TimerCallback && cb)
    : _timer(initialTime, intervalTime, std::move(cb))
    , _thread(std::bind(&Timerfd::start, &_timer))
    {}

    void start()
    {
        _thread.start();
    }

    void stop()
    {
        _timer.stop();
        _thread.join();
    }

private:
    Timerfd _timer;
    Thread _thread;
};

}//end of namespace wd
