/*
brief: use example and test code
*/
#include "../gameUtility/rank/RealTimeRank.h"
#include <algorithm>
#include <iostream>



using namespace std;

namespace
{
	class Member;
	typedef RealTimeRankMgr<Member> MyRealTimeRankMgr;

	class Member
	{
	public:
		Member(uint32 id, int64 score = 0, int64 lv = 0)
			:m_score(score)
			, m_score_rank(0)
			, m_lv(0)
			, m_lv_rank(0)
			, m_id(id)
		{
		}
		void setScore(int64 score, MyRealTimeRankMgr &mgr);
		void setLv(int64 lv, MyRealTimeRankMgr &mgr);

		int64 GetScore(RankType type)
		{
			switch (type)
			{
			default:
				LOG_ERROR("unknow rank type =%d", (int)type);
				assert(false);
				return 0;
			case RankType::LV:		return m_lv;
			case RankType::SCORE:	return m_score;
			}
		}

		int32 GetRankIdx(RankType type)
		{
			switch (type)
			{
			default:
				LOG_ERROR("unknow rank type =%d", (int)type);
				assert(false);
				return 0;
			case RankType::LV:		return m_lv_rank;
			case RankType::SCORE:	return m_score_rank;
			}
		}
		void SetRankIdx(RankType type, int32 idx)
		{
			switch (type)
			{
			default:
				LOG_ERROR("unknow rank type =%d", (int)type);
				assert(false);
				break;
			case RankType::LV:		 m_lv_rank = idx; break;
			case RankType::SCORE:	 m_score_rank = idx; break;
			}
		}
		uint64 GetId(){ return m_id; }
	public:
		int64 m_score; int32 m_score_rank;
		int64 m_lv; int32 m_lv_rank;
		uint32 m_id;
	};




	void Member::setScore(int64 score, MyRealTimeRankMgr &mgr)
	{
		m_score = score;
		mgr.UpdateMemberRank(m_id, RankType::SCORE);
	}

	void Member::setLv(int64 lv, MyRealTimeRankMgr &mgr)
	{
		m_lv = lv;
		mgr.UpdateMemberRank(m_id, RankType::LV);
	}

	const uint32 ARRAY_LEN = 8;


	void printOrder(const RealTimeRank<Member> &rtr)
	{
		const RealTimeRank<Member>::VecMember &vecMember = rtr.GetAllMember();
		if (vecMember.empty())
		{
			assert(false);
			return;
		}
		cout << "id	:";
		for (uint32 i = 0; i < vecMember.size(); ++i)
		{
			Member *p = vecMember[i];
			cout << p->m_id;
			cout << ", ";
		}
		cout << endl;
		cout << "score	:";
		for (uint32 i = 0; i < vecMember.size(); ++i)
		{
			Member *p = vecMember[i];
			cout << p->GetScore(rtr.GetRankType());
			cout << ", ";
		}
		cout << endl;
	}

	void printOrderAndCheckError(const RealTimeRank<Member> &rtr)
	{
		const RealTimeRank<Member>::VecMember &vecMember = rtr.GetAllMember();
		if (vecMember.empty())
		{
			assert(false);
			return;
		}

		printOrder(rtr);

		for (uint32 i = 0; i < vecMember.size(); ++i)
		{
			Member *p = vecMember[i];
			if (i != p->GetRankIdx(rtr.GetRankType()))
			{
				cout << "[error member rank=" << p->GetRankIdx(rtr.GetRankType()) << "]";
				assert(false);
			}
		}

		int64 lastScore = vecMember[0]->GetScore(rtr.GetRankType());
		for (uint32 i = 0; i<vecMember.size(); ++i)
		{
			Member *p = vecMember[i];

			if (p->GetScore(rtr.GetRankType())>lastScore)
			{
				cout << "[error order in pos=" << i << ",";
				assert(false);
			}
			lastScore = p->GetScore(rtr.GetRankType());
		}
		cout << endl;
	}
//���Գ�ʼ��
void testInit()
{
	MyRealTimeRankMgr g_mgr({ RankType::SCORE, RankType::LV });

	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		g_mgr.AddMemberNoRank(Member(i + 1, 10 + i));
	}

	g_mgr.AddMemberNoRank(Member(101, 13));
	g_mgr.AddMemberNoRank(Member(102, 15));

	const RealTimeRank<Member> &rtr = *(g_mgr.GetRank(RankType::SCORE));
	printOrder(rtr);
	Info("sort:");
	g_mgr.Sort();
	printOrderAndCheckError(rtr);
}

//�м��Ա�仯�����
void test1()
{
	MyRealTimeRankMgr g_mgr({ RankType::SCORE, RankType::LV });
	const RealTimeRank<Member> &rtr = *(g_mgr.GetRank(RankType::SCORE));

	const RealTimeRank<Member>::VecMember &g_member = rtr.GetAllMember();
	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		g_mgr.AddMember(*(new Member(i + 1, 0)));
	}
	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		Member *p = g_mgr.GetMember(i + 1);
		p->setScore(i + 1, g_mgr);
	}
	printOrderAndCheckError(rtr);
	printf("set id[%lu]=5\n ", g_member[5]->m_id);
	g_member[5]->setScore(5, g_mgr);
	printOrderAndCheckError(rtr);
	printf("set id[%lu]=3\n ", g_member[5]->m_id);
	g_member[5]->setScore(3, g_mgr);
	printOrderAndCheckError(rtr);
	printf("set id[%lu]=0\n ", 5);
	g_mgr.GetMember(5)->setScore(0, g_mgr);
	printOrderAndCheckError(rtr);
	printf("set id[%lu]=9\n ", 6);
	g_mgr.GetMember(6)->setScore(9, g_mgr);
	printOrderAndCheckError(rtr);
}

	//�������һ���
	void test2()
	{
		MyRealTimeRankMgr g_mgr({ RankType::SCORE, RankType::LV });
		const RealTimeRank<Member> &rtr = *(g_mgr.GetRank(RankType::LV));

		const RealTimeRank<Member>::VecMember &g_member = rtr.GetAllMember();
		for (uint32 i =0; i<ARRAY_LEN; ++i)
		{
			g_mgr.AddMember(*(new Member(i + 1, 0)));
		}
		for (uint32 i =0; i<ARRAY_LEN; ++i)
		{
			Member *p = g_mgr.GetMember(i+1);
			p->setLv(i + 1, g_mgr);
		}

		printOrderAndCheckError(rtr);
		Info("������׵����");
		g_mgr.GetMember(1)->setLv(9, g_mgr);
		printOrderAndCheckError(rtr);
		Info("������ߵ����");
		g_mgr.GetMember(1)->setLv(0, g_mgr);
		printOrderAndCheckError(rtr);
	}
//�������ұ仯�����ھӵȷ�
void test3()
{
	MyRealTimeRankMgr g_mgr({ RankType::SCORE, RankType::LV });
	const RealTimeRank<Member> &rtr = *(g_mgr.GetRank(RankType::SCORE));

	const RealTimeRank<Member>::VecMember &g_member = rtr.GetAllMember();
	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		g_mgr.AddMember(*(new Member(i + 1, 0)));
	}
	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		Member *p = g_mgr.GetMember(i + 1);
		p->setScore(i + 1, g_mgr);
	}

	printOrderAndCheckError(rtr);
	Info("������α仯");
	g_member[0]->setScore(2, g_mgr); 
	printOrderAndCheckError(rtr);
	Info("������α仯");
	g_member[7]->setScore(7, g_mgr);
	printOrderAndCheckError(rtr);
}

//����1��2����Ա�μӵ����
void test4()
{
	MyRealTimeRankMgr g_mgr({ RankType::SCORE, RankType::LV });
	const RealTimeRank<Member> &rtr = *(g_mgr.GetRank(RankType::SCORE));

	const RealTimeRank<Member>::VecMember &g_member = rtr.GetAllMember();
	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		g_mgr.AddMember(*(new Member(i + 1, 0)));
	}
	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		Member *p = g_mgr.GetMember(i + 1);
		p->setScore(i + 1, g_mgr);
	}

	printOrderAndCheckError(rtr);
	Info("��߱仯");
	g_member[0]->setScore(2, g_mgr); 
	printOrderAndCheckError(rtr);
	Info("��ͱ仯");
	g_member[7]->setScore(0, g_mgr);
	printOrderAndCheckError(rtr);


	Info("������߷�");
	Member m(11, 11);
	g_mgr.AddMember(m);
	printOrderAndCheckError(rtr);
	Info("������ͷ�");
	Member m2(12, -1);
	g_mgr.AddMember(m2);
	printOrderAndCheckError(rtr);
	Info("�����м�");
	Member m3(13, 3);
	g_mgr.AddMember(m3);
	printOrderAndCheckError(rtr);
}

//
void test5()
{

}
//
void test6()
{


}
}
void TestRank()
{
	testInit();
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
}

