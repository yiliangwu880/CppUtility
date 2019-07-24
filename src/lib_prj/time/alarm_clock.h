/*
	brief: 闹钟，定时器
	等废弃，svr_util 有更好实现
*/

#pragma once
#include "../utility/typedef.h"
#include "../utility/singleton.h"
#include "../utility/UniqueId.h"
#include "cd.h"
#include <time.h>
#include <vector>
#include <map>

namespace wyl
{

    //注意，如果用para2这种指针参数，指向的内容是否有效依赖用户控制
    typedef void (*TimeCBFun)(uint32 para1, void *para2);
	/*
		定时器，注册回调函数，timeout调用函数。
        注意：通常大部分代码不应该用定时器，经验证明很多问题的。
        ）问题1： 循环定时器，容易没销毁掉旧的继续触发新的循环，越来越多。
	*/
	class TimeCallBack : public SingletonBase<TimeCallBack>
	{
		struct Data
		{
            uint32 id;
            time_t start_sec;
            time_t interval;
            bool is_loop;  //true表示循环定时器
            uint32 para1;
            void *para2;
			TimeCBFun p_fun;
        };
        typedef std::vector<Data> VecData;
		typedef std::multimap<time_t, Data> TimeMapData;   //到期绝对时间 map 数据   需要优化，频繁增加删除会有内存碎片
  
	public:
        //检测timeout事件，执行回调。（一般循环调用这个函数）
		void checkTimeOut();

        //注册定时事件, timeout会回调函数. timer_id在timeout回调前变无效。
        //注意: timer_id无效后，会有新的timer复用这个值。 用户很难控制,使用要小心了。 不建议使用timer_id。 
        //para:time_t interval, timeout间隔
        //return:timer_id。 timeout回调函数以后，会变无效。 
		uint32 setTimer(time_t interval, TimeCBFun p_fun, uint32 para1 = 0, void *para2 = NULL);

        //注册循环定时事件, timeout会回调函数
        //return timer_id
        uint32 setLoopTimer(time_t interval, TimeCBFun p_fun, uint32 para1 = 0, void *para2 = NULL);

        bool delTimer( uint32 timer_id );

        //清所有定时事件
		void clear();

        //获取等待到期的定时器数量
        uint32 GetTimeNum();

    private:

	private:
        static const uint32 MAX_TIMER_NUMBER = 1000; //一般进程定时器超1000就是设计不好。
		TimeMapData m_time_m_data;
        UniqueID m_id_factory;
	};
	extern TimeCallBack &g_timer_cb;

}//namespace wyl