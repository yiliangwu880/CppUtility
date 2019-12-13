/*
brief: use example and test code
*/
#include "../game_utility/rank/exchangeRank.h"
#include <algorithm>
#include <iostream>




using namespace std;

namespace
{

class ConcreteMember : public RankMemberBase
{
public:
	ConcreteMember(uint32 id)
		:m_id(id)
	{
	}
	void id(uint32 id)
	{
		m_id = id;
	}
	uint32 id() const{return m_id;};
	virtual uint32 rank() const{return m_rank;};

	uint32 m_rank;
	uint32 m_id;
protected:
	virtual void rank(uint32 _rank) {m_rank = _rank;}
};

ExchangeRankMgr mgr(5);
map<uint32, ConcreteMember> g_idMapMemeber;

void addNewMember(const ConcreteMember &m)
{
	pair<map<uint32, ConcreteMember>::iterator, bool> ret= g_idMapMemeber.insert(make_pair(m.id(), m));
	assert(ret.second);
	mgr.addNewMember(&(ret.first->second));
}

ConcreteMember *memberFind(RankMemberBase *p)
{
	return dynamic_cast<ConcreteMember *>(p);
}

void exchange(uint32 r1, uint32 r2)
{
	ConcreteMember *p1 = memberFind(mgr.getMember(r1));
	ConcreteMember *p2 = memberFind(mgr.getMember(r2));
	if (NULL == p1 || NULL == p2)
	{
		return;
	}
	mgr.startFight(p1->rank(), p2->rank());
	mgr.endFightExchange(p1->rank(),p2->rank());

}
//test code
//////////////////////////////////////////////////////////////////////////

void printAllRank()
{
	for (uint32 i=0; i<3; ++i)
	{
		ConcreteMember *p = memberFind(mgr.getMember(i));
		uint _id = p->id();
		if (0 == _id)
		{
			cout << i << " : " << "NULL"  << endl;
		} 
		else
		{
			cout << i << " : " <<  _id << endl;
		}
	}
	cout << "------------" << endl;
}


#define FIND_MEMBER_ID(x) memberFind(mgr.getMember(x))->id()
void test1()
{

	addNewMember(ConcreteMember(1));
	addNewMember(ConcreteMember(2));

	assert(FIND_MEMBER_ID(0)==1 && FIND_MEMBER_ID(1)==2);
	exchange(0,1);
	assert(FIND_MEMBER_ID(0)==2 && FIND_MEMBER_ID(1)==1);
	exchange(0,1);
	assert(FIND_MEMBER_ID(0)==1 && FIND_MEMBER_ID(1)==2);
	addNewMember(ConcreteMember(3));
	exchange(0,2);
	assert(FIND_MEMBER_ID(0)==3 && FIND_MEMBER_ID(1)==2 && FIND_MEMBER_ID(2)==1);
	exchange(0,2);
	assert(FIND_MEMBER_ID(0)==1 && FIND_MEMBER_ID(1)==2 && FIND_MEMBER_ID(2)==3);


	printAllRank();
}
void test2()
{
	mgr.clear();
	g_idMapMemeber.clear();

	addNewMember(ConcreteMember(1));
	addNewMember(ConcreteMember(2));
	addNewMember(ConcreteMember(3));

	assert(mgr.enableFight(0,1));
	mgr.startFight(0,1);
	assert(!mgr.enableFight(0,1));
	assert(!mgr.enableFight(0,2));
	assert(!mgr.enableFight(1,2));
	exchange(0,1);
	assert(mgr.enableFight(0,1));

	mgr.startFight(0,1);

	assert(!mgr.enableFight(0,1));
	assert(!mgr.enableFight(0,2));
	assert(!mgr.enableFight(1,2));



	printAllRank();

}
void test3()
{

	mgr.clear();
	g_idMapMemeber.clear();

	addNewMember(ConcreteMember(1));
	addNewMember(ConcreteMember(2));
	addNewMember(ConcreteMember(3));
	addNewMember(ConcreteMember(4));
	mgr.erase(2);

	printAllRank();
}
void test4()
{

	mgr.clear();
	g_idMapMemeber.clear();


	addNewMember(ConcreteMember(1));
	addNewMember(ConcreteMember(2));
	addNewMember(ConcreteMember(3));
	addNewMember(ConcreteMember(4));

	assert(mgr.enableFight(0,1));	
	assert(mgr.enableFight(0,2));
	assert(mgr.enableFight(0,3));
	assert(mgr.enableFight(1,3));
	assert(!mgr.enableFight(1,4));

	assert(!mgr.isFightingPair(0,1));	
	assert(!mgr.isFightingPair(0,2));
	assert(!mgr.isFightingPair(0,3));
	assert(!mgr.isFightingPair(1,3));
	assert(!mgr.isFightingPair(1,4));


	mgr.startFight(0,1);
	assert(!mgr.enableFight(0,1));
	assert(!mgr.enableFight(0,2));
	assert(!mgr.enableFight(0,3));
	assert(!mgr.enableFight(1,3));
	assert(!mgr.enableFight(1,4));

	assert(mgr.isFightingPair(0,1));	
	assert(!mgr.isFightingPair(0,2));
	assert(!mgr.isFightingPair(0,3));
	assert(!mgr.isFightingPair(1,3));
	assert(!mgr.isFightingPair(1, 4));

	assert(mgr.endFightExchange(0, 1, false));

	mgr.startFight(0,2);
	assert(mgr.endFightExchange(0, 2));
	mgr.startFight(0, 3);
	assert(mgr.endFightExchange(0, (3)));
	mgr.startFight(1, 3);
	assert(mgr.endFightExchange((1), (3)));
	mgr.startFight(1, 0);
	assert(mgr.endFightExchange((1), (0)));
	mgr.startFight(0, 1);
	assert(	mgr.endFightExchange( (0), (1) ));
	assert(	!mgr.endFightExchange( 0, 4 ));



	printAllRank();
}

} //end namespace

void testRankFun()
{
	test1();
	test2();
	test3();
	test4();
	Info("done");
}


