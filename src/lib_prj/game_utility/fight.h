/*
供参考，没复用价值
brief:游戏战斗相关
对象分解：技能，技能效果，效果作用对象, 日志

加战斗记录
加区分施放处理效果结果

buf保存用mask的方式，方便定位查找

map 查找的buf,附带效果，改成vector直接定位查找。增加内存提高效率
最后分析：新工程，加入新需求，怎么改动才能使用
*/

#pragma once
#include "../utility/typedef.h"
#include "../utility/memoryStream.h"
#include <vector>
#include <map>

//效果类型
//PCT = percent
//PSC = physical
//DAM = damage
enum EffectType
{
	ET_NONE,
	ET_SUCK_LIFE, 
	ET_SUCK_LIFE_BUF, 
	ET_ADD_LIFE,
	ET_PSC_DAM,
	ET_ADD_PCT_PSC_DAM,
	ET_DEC_PSC_DAM,
	ET_REFLECT,
	ET_RE_PSC_DAM,
	ET_RE_PSC_DAM_BUF,
	ET_IGNORE_DEF,  //忽略防御
	ET_LICH_UNIQUE,  //巫妖的大

	//新加效果类型
};

struct EffectData 
{
	EffectData(){};
	EffectData(EffectType _type, uint32 _value):type(_type), v(_value){};
	EffectType type;
	uint32 v;
};


typedef std::map<EffectType, uint32> EffectTypeMapV;
struct Effects
{
	Effects(EffectType _type, uint32 _value):main(_type, _value){};

	EffectData main;		//主效果
	EffectTypeMapV append;  //附带效果
};

struct EffectBufTime
{
};

struct EffectBuf
{
	EffectType type;
	union
	{
		struct  
		{
			uint32 count;
			EffectBufTime time; //持续时间 
		};
	};
};
typedef std::vector<EffectBuf> VecBuf;
typedef std::map<EffectType, uint32> EffectTypeMapBuf;  

class SkillBase
{
public:

};

class EffectBase
{
public:

};

class TargetBase;

struct LaunchDate
{
	LaunchDate(const Effects &_effect, TargetBase &_er, Memory &_log)
		:effects(_effect)
		,er(_er)
		,log(_log)
	{
	}
	const Effects &effects;
	TargetBase &er;
	Memory &log;
};

class TargetBase
{
public:
	virtual ~TargetBase(){};
};

//新项目要修改代码
//////////////////////////////////////////////////////////////////////////
class FightCalculate
{
public:
	//技能施放逻辑
	void launch(const Effects &_effect, TargetBase &er, TargetBase &target, Memory &log);
private:
};

//巫妖的大技能，作为一个目标对象
class LichBall : public TargetBase
{
public:
	uint32 getRemain(){return m_remain;}
private:
	uint32 m_remain; //剩余反弹

};

class FightTarget : public TargetBase
{


public:
private:
	VecBuf::iterator find(EffectType type);

private:
	VecBuf m_buf;

};


