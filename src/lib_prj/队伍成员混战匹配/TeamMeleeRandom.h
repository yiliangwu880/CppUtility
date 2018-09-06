/*
brief:队伍成员混战随机生成
具体随机步骤：
-1）输入所有混战成员。
1） 随机所有队伍的成员在本队的出战顺序。
2） 优先随机上一轮没出战的成员。
3） 随机两个不同队的成员做一对。
4） 重复3）直到无法随机出一对。
5） 随机一轮结束，保存未能出战的成员。
6） 外部战斗结束，输入幸存者到作为混战成员。
7） 如果只有一队幸存，结束。不然跳到1)步骤。

ID == 0 为无效值
*/
#pragma once
#include <vector>
#include <map>
#include <utility>

typedef unsigned int uint32;

//混战随机一轮的所有匹配
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
