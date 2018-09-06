/*
brief:支持多线程访问消息队列
*/

#pragma once

#include "pthread.h"

//参考用吧，还是直接copy代码在里面加条件好用。复用这个类代码太复杂。
class ThreadSleepWait
{
public:
    ThreadSleepWait()
    {
        pthread_cond_init(&m_cond, NULL);
        pthread_mutex_init(&m_mutex, NULL);
    }

    //CondFun is_wait,函数对象，要求需要等待的时候返回true
    template<class CondFun>
    void wait(CondFun &is_wait)
    {
        pthread_mutex_lock(&m_mutex);
        while (is_wait())
        {
            pthread_cond_wait(&m_cond, &m_mutex);
        }
        pthread_mutex_unlock(&m_mutex);
    }

    void end_wait()
    {
        pthread_cond_signal(&m_cond);
    }

private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};


