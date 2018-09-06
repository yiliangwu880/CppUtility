/*
brief:�����Ա��ս�������
����������裺
-1���������л�ս��Ա��
1�� ������ж���ĳ�Ա�ڱ��ӵĳ�ս˳��
2�� ���������һ��û��ս�ĳ�Ա��
3�� ���������ͬ�ӵĳ�Ա��һ�ԡ�
4�� �ظ�3��ֱ���޷������һ�ԡ�
5�� ���һ�ֽ���������δ�ܳ�ս�ĳ�Ա��
6�� �ⲿս�������������Ҵ��ߵ���Ϊ��ս��Ա��
7�� ���ֻ��һ���Ҵ棬��������Ȼ����1)���衣

ID == 0 Ϊ��Чֵ
*/
#pragma once
#include <vector>
#include <map>
#include <utility>

typedef unsigned int uint32;

//��ս���һ�ֵ�����ƥ��
class TeamMeleePair
{
public:
	struct Member 
	{
		uint32 teamId;
		uint32 memberId;
	};
	struct Pair 
	{
		Member first, second;
	};
	
	typedef std::vector<Pair> VecPair;
	VecPair m_vecPair;

	void clear();
	void add(uint32 teamId1, uint32 memberId1, uint32 teamId2, uint32 memberId2 );
};

class TeamMeleeRandom
{
public:
	typedef std::vector<uint32> VecId;
	typedef std::map<uint32, VecId> MapTeam;

	void clear();
	void addCandidate(uint32 teamId, uint32 memberId);
	//bref:return false if only one team remain.
	bool random(TeamMeleePair &teamMeleePair);
	bool enableRandom() const; //return false is is Only One Team Remain
	void getRemain(uint32 &teamId, VecId &vecMember) const;

private:
	void updateLastNoMatchMember();
	void preferRandomRemain(TeamMeleePair &teamMeleePair);
	void randomMemberOrderInTeam();
	void randomTeamOrder(VecId &teamOrder) const;
	void randomTeamOrderByExcept(VecId &teamOrder, uint32 exceptTeamId) const;
private:

	MapTeam m_mapTeam;
	uint32 m_lastNoMatchTeamId;
	VecId m_lastNoMatchMember;
};
