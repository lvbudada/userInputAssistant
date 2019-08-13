#pragma once

#include "Noncopyable.h"

#include <pthread.h>
#include <functional>
#include <string>
using std::function;
using std::string;

namespace wd
{

namespace current_thread
{
    extern __thread const char * threadName;
}

class Thread
: Noncopyable
{
public:
    using ThreadCallback = function<void()>;

    Thread(ThreadCallback && cb, const string & name = string())
    : _pthid(0)
    , _isRuning(false)
    , _cb(std::move(cb))
    , _name(name)
    {}

    void start();
    void join();

    ~Thread();
private:
    static void* threadfunc(void *arg);
private:
    pthread_t _pthid;
    bool _isRuning;
    ThreadCallback _cb;
    string _name;
};

}//end of namespace wd
