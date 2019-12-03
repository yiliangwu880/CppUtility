/*
brief:游戏战斗相关

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
		//加一次吸血BUF
		break;
	case ET_IGNORE_DEF:
		break;
	case ET_PSC_DAM:
		{
			uint32 dam=0, def=0, suck_life=0;
			EffectTypeMapBuf buf;  //target buf
			//buf = target.getBuf();

			//计算防御
			//.....

			//忽略防御
			{
				EffectTypeMapV::const_iterator it = append.find(ET_IGNORE_DEF);
				if (it!=append.end())
				{
					def=0;
				}
			}

			//计算伤害
			//.....

			//反弹伤害
			{
				EffectTypeMapBuf::const_iterator it = buf.find(ET_REFLECT);
				if (it!=buf.end())
				{
					//直接伤害目标，不算防御
					//er.re_psc_dam();
				}
			}

			//吸血
			{
				EffectTypeMapV::const_iterator it = append.find(ET_SUCK_LIFE);
				if (it!=append.end())
				{
					suck_life=it->second;
				}
			}

			//计算吸血量
			//....
			//er.addHp(xx);	
		}
		break;
	case ET_LICH_UNIQUE:
		{
			//反弹数减少
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
			{//反弹另一个目标
				FightTarget other;
				Effects t(ET_LICH_UNIQUE,0);
				launch(t, er, target, log);
			}
		}
		break;
	}
}