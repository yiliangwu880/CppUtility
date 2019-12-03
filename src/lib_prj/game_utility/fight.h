/*
���ο���û���ü�ֵ
brief:��Ϸս�����
����ֽ⣺���ܣ�����Ч����Ч�����ö���, ��־

��ս����¼
������ʩ�Ŵ���Ч�����

buf������mask�ķ�ʽ�����㶨λ����

map ���ҵ�buf,����Ч�����ĳ�vectorֱ�Ӷ�λ���ҡ������ڴ����Ч��
���������¹��̣�������������ô�Ķ�����ʹ��
*/

#pragma once
#include "../utility/typedef.h"
#include "../utility/memoryStream.h"
#include <vector>
#include <map>

//Ч������
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
	ET_IGNORE_DEF,  //���Է���
	ET_LICH_UNIQUE,  //�����Ĵ�

	//�¼�Ч������
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

	EffectData main;		//��Ч��
	EffectTypeMapV append;  //����Ч��
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
			EffectBufTime time; //����ʱ�� 
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

//����ĿҪ�޸Ĵ���
//////////////////////////////////////////////////////////////////////////
class FightCalculate
{
public:
	//����ʩ���߼�
	void launch(const Effects &_effect, TargetBase &er, TargetBase &target, Memory &log);
private:
};

//�����Ĵ��ܣ���Ϊһ��Ŀ�����
class LichBall : public TargetBase
{
public:
	uint32 getRemain(){return m_remain;}
private:
	uint32 m_remain; //ʣ�෴��

};

class FightTarget : public TargetBase
{


public:
private:
	VecBuf::iterator find(EffectType type);

private:
	VecBuf m_buf;

};


