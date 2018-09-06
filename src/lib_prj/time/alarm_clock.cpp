/*
时间相关
*/

#include "alarm_clock.h"
#include <assert.h>
#include <string>
#include "../utility/misc.h"
#include "sys_time.h"
#include <utility>
#include <time.h>


using namespace std;
namespace wyl
{
	TimeCallBack &g_timer_cb = TimeCallBack::obj();
    using namespace std;


	uint32 TimeCallBack::setTimer(time_t interval, TimeCBFun p_fun, uint32 para1, void *para2)
	{
		if (NULL == p_fun)
		{
			return 0;
		}
        if (m_time_m_data.size() >= MAX_TIMER_NUMBER)
        {
            printf("timer num is over load!!\n");
            return 0;
        }

		time_t sec = g_time.Sec();
		time_t key = sec+interval;
		if (key<sec)//时间太大溢出，好几千年都轮不到。
		{
			return 0;
		}

        Data d;
        d.id = m_id_factory.newId();
        d.start_sec = sec;
        d.interval = interval;
        d.is_loop = false;
        d.para1 = para1;
        d.para2 = para2;
		d.p_fun = p_fun;

        m_time_m_data.insert(make_pair(key, d));
        return d.id;
	}

	uint32 TimeCallBack::setLoopTimer(time_t interval, TimeCBFun p_fun, uint32 para1, void *para2)
	{
		if (NULL == p_fun)
		{
			return 0;
        }
        if (m_time_m_data.size() >= MAX_TIMER_NUMBER)
        {
            printf("timer num is over load!!\n");
            return 0;
        }

        time_t sec = g_time.Sec();
		time_t key = sec+interval;
		if (key<sec)//时间太大溢出，好几千年都轮不到。
		{
			return 0;
		}

        Data d;
        d.id = m_id_factory.newId();
        d.start_sec = sec;
        d.interval = interval;
        d.is_loop = true;
        d.para1 = para1;
        d.para2 = para2;
		d.p_fun = p_fun;

         m_time_m_data.insert(make_pair(key, d));
        return d.id;
	}

    bool TimeCallBack::delTimer( uint32 timer_id )
    {
        bool ret = false;
        for (TimeMapData::iterator it_1=m_time_m_data.begin(); it_1!=m_time_m_data.end(); )
        {
            Data &d = it_1->second;
            if (d.id == timer_id)
            {
                m_time_m_data.erase(it_1++);
                m_id_factory.delId(d.id);
                ret = true;
            }
            else
            {
                ++it_1;
            }
        }	
        return ret;
    }

	void TimeCallBack::checkTimeOut()
	{
		time_t sec = g_time.Sec();
        VecData vec_timeout;
		for (TimeMapData::iterator it_1=m_time_m_data.begin(); it_1!=m_time_m_data.end(); )
		{
			if(it_1->first > sec) 
			{
				break;
			}
			//time out
            const Data &d = it_1->second;
            vec_timeout.push_back(d);
            m_time_m_data.erase(it_1++);
        }
        
        //重设循环的timer
        FOR_IT(VecData, vec_timeout)
        {
            if (it->is_loop)
            {
                it->start_sec += it->interval;
                m_time_m_data.insert(make_pair(it->start_sec+it->interval, *it));
            }
            else
            {
                m_id_factory.delId(it->id);
            }
        }

        //保证最后调用，防止回调的时候，调用接口改变成员。
        CONST_FOR_IT(VecData, vec_timeout)
        {
            (*it->p_fun)(it->para1, it->para2);
        }
	}

	void TimeCallBack::clear()
	{
		m_time_m_data.clear();
        m_id_factory.reset();
	}

    uint32 TimeCallBack::GetTimeNum()
    {
        return m_time_m_data.size();
    }



}