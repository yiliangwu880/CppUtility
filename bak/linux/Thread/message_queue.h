/*
brief:֧�ֶ��̷߳�����Ϣ����
*/

#pragma once

#include "pthread.h"

//�ο��ðɣ�����ֱ��copy������������������á�������������̫���ӡ�
class ThreadSleepWait
{
public:
    ThreadSleepWait()
    {
        pthread_cond_init(&m_cond, NULL);
        pthread_mutex_init(&m_mutex, NULL);
    }

    //CondFun is_wait,��������Ҫ����Ҫ�ȴ���ʱ�򷵻�true
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


