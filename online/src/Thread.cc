#include "Thread.h"

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

namespace current_thread
{
    __thread const char * threadName = "wd thread";
}

void Thread::start()
{
    pthread_create(&_pthid, nullptr, threadfunc, this);
    cout << "_pthid = " << _pthid << endl;
    _isRuning = true;
}

void* Thread::threadfunc(void *arg)
{
    Thread * pthread = static_cast<Thread*>(arg);

    current_thread::threadName = pthread->_name == string() ? "wd thread" : pthread->_name.c_str();
    if(pthread)
    {
        pthread->_cb();
    }

    return nullptr;
}

void Thread::join()
{
    if(_isRuning)
        pthread_join(_pthid, nullptr);
}

Thread::~Thread()
{
    if(_isRuning)
    {
        pthread_detach(_pthid);
    }
    cout << "~Thread()" << endl;
}


}//end of namespace wd
