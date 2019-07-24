/*
	brief: ���ӣ���ʱ��
	�ȷ�����svr_util �и���ʵ��
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

    //ע�⣬�����para2����ָ�������ָ��������Ƿ���Ч�����û�����
    typedef void (*TimeCBFun)(uint32 para1, void *para2);
	/*
		��ʱ����ע��ص�������timeout���ú�����
        ע�⣺ͨ���󲿷ִ��벻Ӧ���ö�ʱ��������֤���ܶ�����ġ�
        ������1�� ѭ����ʱ��������û���ٵ��ɵļ��������µ�ѭ����Խ��Խ�ࡣ
	*/
	class TimeCallBack : public SingletonBase<TimeCallBack>
	{
		struct Data
		{
            uint32 id;
            time_t start_sec;
            time_t interval;
            bool is_loop;  //true��ʾѭ����ʱ��
            uint32 para1;
            void *para2;
			TimeCBFun p_fun;
        };
        typedef std::vector<Data> VecData;
		typedef std::multimap<time_t, Data> TimeMapData;   //���ھ���ʱ�� map ����   ��Ҫ�Ż���Ƶ������ɾ�������ڴ���Ƭ
  
	public:
        //���timeout�¼���ִ�лص�����һ��ѭ���������������
		void checkTimeOut();

        //ע�ᶨʱ�¼�, timeout��ص�����. timer_id��timeout�ص�ǰ����Ч��
        //ע��: timer_id��Ч�󣬻����µ�timer�������ֵ�� �û����ѿ���,ʹ��ҪС���ˡ� ������ʹ��timer_id�� 
        //para:time_t interval, timeout���
        //return:timer_id�� timeout�ص������Ժ󣬻����Ч�� 
		uint32 setTimer(time_t interval, TimeCBFun p_fun, uint32 para1 = 0, void *para2 = NULL);

        //ע��ѭ����ʱ�¼�, timeout��ص�����
        //return timer_id
        uint32 setLoopTimer(time_t interval, TimeCBFun p_fun, uint32 para1 = 0, void *para2 = NULL);

        bool delTimer( uint32 timer_id );

        //�����ж�ʱ�¼�
		void clear();

        //��ȡ�ȴ����ڵĶ�ʱ������
        uint32 GetTimeNum();

    private:

	private:
        static const uint32 MAX_TIMER_NUMBER = 1000; //һ����̶�ʱ����1000������Ʋ��á�
		TimeMapData m_time_m_data;
        UniqueID m_id_factory;
	};
	extern TimeCallBack &g_timer_cb;

}//namespace wyl