/************************************************************************/
/*
Author:         Yiliangwu
*/
/************************************************************************/

#pragma once

#include "../../svr_util/include/typedef.h"
#include <vector>

typedef  unsigned short U16;
/* ---------------------------------------------------------------------------
    RANDOM NUMBER GENERATOR
    This library provide the functions to generate random number sequence,
    all the number in this sequence will not repeat within a round
	
	
   ------------------------------------------------------------------------ */
class PrimeRandom
{
public:	
	//����U16 maxNum��ʾ ���������������0��ʼ�� �������=2��ֻ�������0,1
	explicit PrimeRandom(U16 maxNum);
	U16 GetRandom();
private:
	void Init(U16 maxNum);
	U16 oRandom( U16 range );
	void RandomMode();
	void GetRightNext();
	void GetLeftNext();
	U16 GetNext(bool isLeft);



private:
	U16 m_modeStartIndex;
	U16 m_step;
	U16 m_mode;
	U16 m_right;
	U16 m_left;
	U16 m_start;
	U16 m_base;

	static const U16 PrimeNumbersTable[];
	static const size_t REMAIN_RANG = 6;
	static const U16 MAX_PRIME;
	static const U16 PRIME_LEN;
};


/************************************************************************/
/* 
����Ȩ�����������[0]=10, [1]=90.���1�ļ�����90%
*/
/************************************************************************/
class RandomWeight
{
public:
	RandomWeight(const std::vector<uint32> &vecWeight);
	uint32 rand();	
	//para vecWeight Ϊ���Ȩ������
	//return ��������0��ʼ
	static uint32 rand(const std::vector<uint32> &vecWeight);

private:
	std::vector<uint32> m_sumWeight;
	uint32 m_sum;
};

namespace wyl
{
	void InitSeed();
	//�����������һ����, ��ȱ�㣬 �����������
	uint32 randBetween(uint32 min, uint32 max);
	int GetRandomNumber(int iMin, int iMax);//�����������һ����

    uint32 rand32();
    // ����[low, high]֮������ظ���cnt�������
    //��������ȴ�Ч�ʻ�͡�������PrimeRandom��
    bool GetIntervalNoRepeatedRandNum(U16 low, U16 high, U16 cnt, U16 *num);
}//namespace wyl