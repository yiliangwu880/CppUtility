

#include "stdafx.h"
#include "../Random/randomName.h"
#include "../Random/random.h"



using namespace std;
using namespace wyl;
namespace
{

void testRandomName()
{
	RandomPeerUse<PrimeRandom> m_ran(5);

	int i = 10;
	while (i--)
	{
		Info(m_ran.peer());
	}
	i = 2;

	m_ran.use(2);
	m_ran.use(4);
	Info("--------------24---");
	Info(m_ran.peer());
	Info(m_ran.peer());
	Info(m_ran.peer());
	Info(m_ran.peer());
	Info(m_ran.peer());
	Info(m_ran.peer());
}

void testRandomWeight()
{
	vector<uint32> vec;
	vec.resize(3);
	{
		vec[0]=0;
		vec[1]=0;
		vec[2]=1;
		RandomWeight obj(vec);
		assert(2==obj.rand());
		assert(2==obj.rand());
		assert(2==obj.rand());
		assert(2==obj.rand());
	}
	{
		vec[0]=1;
		vec[1]=1;
		vec[2]=1;
		RandomWeight obj(vec);
		assert(obj.rand()<3);
		assert(obj.rand()<3);
		assert(obj.rand()<3);
		assert(obj.rand()<3);
		assert(obj.rand()<3);
		assert(obj.rand()<3);
	}
	{
		vec[0]=1;
		vec[1]=10;
		vec[2]=1;
		RandomWeight obj(vec);
		Info("vaule=1 is most result");
		Info(obj.rand());
		Info(obj.rand());
		Info(obj.rand());
		Info(obj.rand());
		Info(obj.rand());
		Info("vaule=1 is most result");
		Info(obj.rand(vec));
		Info(obj.rand(vec));
		Info(obj.rand(vec));
		Info(obj.rand(vec));
		Info(obj.rand(vec));
		Info(obj.rand(vec));
	}

}
void testPrimeRandom()
{
	PrimeRandom r(1000);
	std::set<uint16> s;
	for (uint32 i=0; i<1000; i++)
	{
		s.insert(r.GetRandom());
	}
	uint16 count = 0;
	for (std::set<uint16>::const_iterator it=s.begin(); it!=s.end(); ++it, ++count)
	{
		assert(*it==count);
	}
	assert(1000==count);

	std::set<uint16> s2;
	for (uint32 i = 0; i < 1000; i++)
	{
		s2.insert(r.GetRandom());
	}
	assert(1000 == s2.size());
}
void  testRandomBetween()
{
	bool isOne = false;
	bool isZero = false;
	for (uint32 i=0; i<1000; ++i)
	{
		switch(wyl::randBetween(1,0))
		{
		default:
			assert(false);
			break;
		case 0:
			isZero = true;
			break;
		case 1:
			isOne = true;
			break;
		}
	}
	assert(isOne);
	assert(isZero);

	for (uint32 i=0; i<1000; ++i)
	{
		uint32 r = wyl::randBetween(0,100);
		assert(r>=0 && r<=100);
	}
}

void test()
{
	int r = 0;
	assert(GetRandomNumber(0, 0) == 0);
	assert(GetRandomNumber(-1, -1) == -1);
	for (uint32 i = 0; i < 1000; ++i)
	{
		r = GetRandomNumber(0, 1);
		assert((r == 0) || (1 == r));
	}
}
}//end namespace

void testRandom()
{	
	testPrimeRandom();
	testRandomName();
	testRandomWeight();
	testRandomBetween();
	test();
	Info("done");
}