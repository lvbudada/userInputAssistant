#pragma once
#include <functional>
using std::function;

namespace wd
{

class Timerfd
{
public:
    using TimerCallback = function<void()>;

    Timerfd(int initialTime, int intervalTime, TimerCallback && cb);

    void start();
    void stop();

private:
    int createTimerfd();
    void setTimerfd(int initialTime, int intervalTime);
    void handleRead();

private:
    int _fd;
    int _initialTime;
    int _intervalTime;
    TimerCallback _cb;
    bool _isStarted;
};

}//end of namespace wd
