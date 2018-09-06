#include "TeamMeleeRandom.h"
#include <algorithm>
#include <cassert>
#include <iostream>


using namespace std;


TeamMeleeRandom teamRandom;
TeamMeleePair teamMeleePair;


void printPair()
{
	TeamMeleePair::VecPair::const_iterator iter = teamMeleePair.m_vecPair.begin();
	for (; iter!=teamMeleePair.m_vecPair.end(); ++iter)
	{
		cout << iter->first.teamId << ":" << iter->second.teamId << "---";
		cout << iter->first.memberId << ":" << iter->second.memberId << endl;
	}
}

void randomWinner()
{
	cout << "addWinner:";
	TeamMeleePair::VecPair::const_iterator iter = teamMeleePair.m_vecPair.begin();
	for (; iter!=teamMeleePair.m_vecPair.end(); ++iter)
	{
		teamRandom.addCandidate(iter->first.teamId, iter->first.memberId);
		cout << iter->first.memberId << ", ";
	}
	cout  << endl;
}

void TestTeamMeleeRandomAndPrint()
{
	while (teamRandom.random(teamMeleePair))
	{
		printPair();
		randomWinner();
	}
	TeamMeleeRandom::VecId vecId;
	uint32 teamId;
	teamRandom.getRemain(teamId, vecId);
	cout << "remain:";
	TeamMeleeRandom::VecId::const_iterator iter = vecId.begin();
	for (; iter!=vecId.end(); ++iter)
	{
		cout << *iter << ", ";
	}
	cout << endl;
	teamRandom.clear();
}



void TestTeamMeleeRandom1()
{
	Info("test1------------------");
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom2()
{
	Info("test2------------------");
	teamRandom.addCandidate(1, 11);
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom3()
{
	Info("test3------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(1, 12);
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom4()
{
	Info("test4------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(2, 21);
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom5()
{
	Info("test5------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(2, 21);
	teamRandom.addCandidate(2, 22);
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom6()
{
	Info("test6------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(2, 21);
	teamRandom.addCandidate(3, 31);
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom7()
{
	Info("test7------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(2, 21);
	teamRandom.addCandidate(3, 31);
	teamRandom.addCandidate(4, 41);
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom8()
{
	Info("test8------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(2, 21);
	teamRandom.addCandidate(3, 31);
	teamRandom.addCandidate(4, 41);
	teamRandom.addCandidate(5, 51);
	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom9()
{
	Info("test9------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(1, 12);
	teamRandom.addCandidate(1, 13);
	teamRandom.addCandidate(2, 21);
	teamRandom.addCandidate(3, 31);
	teamRandom.addCandidate(4, 41);
	teamRandom.addCandidate(5, 51);
	TestTeamMeleeRandomAndPrint();
}

void TestTeamMeleeRandom10()
{
	Info("test10------------------");
	teamRandom.addCandidate(1, 11);
	teamRandom.addCandidate(1, 12);
	teamRandom.addCandidate(1, 13);
	teamRandom.addCandidate(1, 14);

	teamRandom.addCandidate(2, 21);
	teamRandom.addCandidate(2, 22);


	TestTeamMeleeRandomAndPrint();
}
void TestTeamMeleeRandom()
{
	TestTeamMeleeRandom1();
	TestTeamMeleeRandom2();
	TestTeamMeleeRandom3();
	TestTeamMeleeRandom4();
	TestTeamMeleeRandom5();
	TestTeamMeleeRandom6();
	TestTeamMeleeRandom7();
	TestTeamMeleeRandom8();
	TestTeamMeleeRandom9();
	TestTeamMeleeRandom10();

}
