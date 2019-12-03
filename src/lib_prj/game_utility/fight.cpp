/*
brief:��Ϸս�����

*/

#include "fight.h"
#include "../utility/cnt_typedef.h"
#include <algorithm>

using namespace std;



void FightCalculate::launch( const Effects &effects, TargetBase &er, TargetBase &target, Memory &log )
{
	const EffectTypeMapV &append = effects.append;
	switch(effects.main.type)
	{
	default:
		return;
	case ET_SUCK_LIFE:
		//��һ����ѪBUF
		break;
	case ET_IGNORE_DEF:
		break;
	case ET_PSC_DAM:
		{
			uint32 dam=0, def=0, suck_life=0;
			EffectTypeMapBuf buf;  //target buf
			//buf = target.getBuf();

			//�������
			//.....

			//���Է���
			{
				EffectTypeMapV::const_iterator it = append.find(ET_IGNORE_DEF);
				if (it!=append.end())
				{
					def=0;
				}
			}

			//�����˺�
			//.....

			//�����˺�
			{
				EffectTypeMapBuf::const_iterator it = buf.find(ET_REFLECT);
				if (it!=buf.end())
				{
					//ֱ���˺�Ŀ�꣬�������
					//er.re_psc_dam();
				}
			}

			//��Ѫ
			{
				EffectTypeMapV::const_iterator it = append.find(ET_SUCK_LIFE);
				if (it!=append.end())
				{
					suck_life=it->second;
				}
			}

			//������Ѫ��
			//....
			//er.addHp(xx);	
		}
		break;
	case ET_LICH_UNIQUE:
		{
			//����������
			{
			//	er.decCount();
			}
			//do damage
			//check if enable rebound 
			LichBall *p_lich_ball = dynamic_cast<LichBall *>(&er);
			if (NULL != p_lich_ball)
			{
				p_lich_ball->getRemain();
			}
			if(0)
			{//������һ��Ŀ��
				FightTarget other;
				Effects t(ET_LICH_UNIQUE,0);
				launch(t, er, target, log);
			}
		}
		break;
	}
}