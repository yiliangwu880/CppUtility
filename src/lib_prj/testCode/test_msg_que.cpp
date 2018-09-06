/*
brief:支持多线程访问消息队列
*/


#include "sys/socket.h"
#include "../Thread/message_queue.h"
#include "../utility/circleBuf.h"
#include "../Thread/Thread.h"
#include "misc.h"
#include <vector>
#include "stdio.h"

using namespace wyl;
using namespace std;

namespace
{
    typedef CircleBuf<char,10> MsgQue;
    ThreadSleepWait g_th_wait;
    MsgQue g_msg_que;
    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;
    const unsigned TEST_NUM = 100;

	//想过， 不能直接用简单条件变量。 会导致死等。
	//写线程满的时候，睡觉， 
	//读线程，不用互斥的情况，会有可能检查不到满。（比如10满，读9过程中时候，检查不满，这时候切另一个线程填10，睡觉，切读线程，读完，结果剩9.  等下次读的时候检查到不满， 导致写线程一直睡觉）
    class ReadThread : public Thread
    {
    public:
        bool is_stop;
        ReadThread():is_stop(false)
        {
        }
        virtual void stop()
        {
            is_stop = true;
        }

        virtual void *run ()
        {
            Info("start ReadThread");
            unsigned vec_char[TEST_NUM]={0};
            unsigned notify_cnt = 0;
            unsigned read_cnt = 0;
            while (1)
            {
                bool is_notify = false;

                if (g_msg_que.full())
                {
                    is_notify =  true;
                }

                if (!g_msg_que.empty())
                {
                    assert(read_cnt<ArrayLen(vec_char));
					char t;
					g_msg_que.pop(t);
					vec_char[read_cnt] = t;
                    read_cnt++;
                }
                
                if(is_notify)
                {
                    pthread_cond_signal(&m_cond);
                    notify_cnt++;
                }

                if (TEST_NUM == read_cnt)
                {
                    break;
                }
            }
            unsigned i=0;
            for (; i<ArrayLen(vec_char); ++i)
            {
                assert(vec_char[i] == i);
            }
            assert(TEST_NUM == i);
            printf("ReadThread end, notify_cnt=%d\n", notify_cnt);
            return 0;
        }
    };


    void testMoreThread()
    {
        ReadThread thr;
        assert(thr.start());

        //本线程写,连续写1W
        for (unsigned i=0; i<TEST_NUM;)
        {
            if (g_msg_que.full())
            {
                pthread_mutex_lock(&m_mutex);
                while (g_msg_que.full())
                {
                    pthread_cond_wait(&m_cond, &m_mutex);
                }
                pthread_mutex_unlock(&m_mutex);
            }
            else
            {
                assert(g_msg_que.push(i));
                ++i;
            }
        }
        Info("write end, wait read end");
        thr.jion();
    }

}//end namespace

void testMsgQue()
{
    Info("============\testMsgQue start============\n");
    testMoreThread();
    Info("============\testMsgQue done================\n");
}


