
#include "../utility/region.h"
#include "../utility/easyTest.h"
#include "../utility/memoryStream.h"



using namespace std;

namespace
{
	void test1()
	{

		{
			MapRegion m;
			MapRegion::Region region;
			assert(m.addRegion(5, 10));

			assert(m.find(10, region));
			assert(5 == region.first);
			assert(10 == region.second);

			assert(m.find(5, region));
			assert(5 == region.first);
			assert(10 == region.second);
		}
		MapRegion m;
		MapRegion::Region region;

		assert( !m.find(1, region) );
		assert( !m.find(0, region) );
		assert( !m.find(12, region) );
		assert(m.addRegion(0,0));
		assert(!m.addRegion(0,13));
		assert(!m.addRegion(11,10));


		assert(m.addRegion(10,10));
		assert( !m.find(1, region) );
		assert( !m.find(11, region) );
		assert( m.find(0, region) );
		assert(m.find(10, region));
		assert( 10 == region.first );
		assert(!m.erase(11));
		assert(m.erase(10));


		assert( !m.find(1, region) );
		assert( m.find(0, region) );
		assert( !m.find(12, region) );
		assert(!m.addRegion(0,0));
		assert(!m.addRegion(0,13));
		assert(!m.addRegion(11,10));

		m.clear();
		//clear at current state

		assert(m.addRegion(10,10));
		assert(!m.addRegion(10,11));
		assert(!m.addRegion(10,12));
		assert(!m.addRegion(9,10));
		assert(!m.addRegion(9,11));
		assert(!m.addRegion(0,10));
		assert(!m.addRegion(0,11));
		assert( m.find(10, region) );assert( 10 == region.first );

		//state = [10,10]
		assert(m.addRegion(11,11));
		assert(!m.addRegion(11,12));
		assert(!m.addRegion(11,13));
		assert(!m.addRegion(9,11));
		assert(!m.addRegion(9,12));
		assert(!m.addRegion(0,11));
		assert(!m.addRegion(0,12));
		assert(!m.addRegion(10,11));

		assert( m.find(10, region) );assert( 10 == region.first );

		//state = [10,10], [11,11]
		assert(m.addRegion(5,7));

		//state = [5,7], [10,10], [11,11]
		assert(!m.addRegion(5,5));
		assert(!m.addRegion(5,6));
		assert(!m.addRegion(5,7));
		assert(!m.addRegion(5,8));
		assert(!m.addRegion(5,10));
		assert(!m.addRegion(0,10));
		assert(!m.addRegion(4,7));
		assert(!m.addRegion(7,8));
		assert(!m.addRegion(0,5));
		assert(!m.addRegion(4,5));

		assert( m.find(5, region) );assert( 5 == region.first );
		assert( m.find(6, region) );assert( 5 == region.first );
		assert( m.find(7, region) );assert( 5 == region.first );
		assert( !m.find(8, region) );
		assert( !m.find(9, region) );

		assert(m.addRegion(8,9));
		assert(m.addRegion(1,2));
		//state = [1,2], [5,7], [8,9], [10,10], [11,11]
		assert( m.find(5, region) );assert( 5 == region.first );
		assert( m.find(6, region) );assert( 5 == region.first );
		assert( m.find(7, region) );assert( 5 == region.first );
		assert( m.find(8, region) );assert( 8 == region.first );
		assert( m.find(9, region) );assert( 8 == region.first );
		assert( m.find(1, region) );assert( 1 == region.first );
		assert( m.find(2, region) );assert( 1 == region.first );
		assert( !m.find(3, region) );
		assert( !m.find(13, region) );

		m.clear();

		assert(m.addRegion(8,9));
		assert(m.addRegion(10,20));
		assert( m.find(11, region) );assert( 10 == region.first );



	}

	void test2()
	{
		MapRegion m;
		assert(m.addRegion(0,10));
		assert(m.addRegion(11,20));
		assert(m.addRegion(30,40));
		assert(m.addRegion(50,60));

		MapRegion::VecSpan vec_span;
		m.getSpan(1,3,vec_span);
		assert(1 == vec_span.size());
		assert(0 == vec_span[0].lower && 2 == vec_span[0].dist);
		m.getSpan(0,3,vec_span);
		assert(1 == vec_span.size());
		assert(0 == vec_span[0].lower && 3 == vec_span[0].dist);
		m.getSpan(1,10,vec_span);
		assert(1 == vec_span.size());
		assert(0 == vec_span[0].lower && 9 == vec_span[0].dist);
		m.getSpan(0,10,vec_span);
		assert(1 == vec_span.size());
		assert(0 == vec_span[0].lower && 10 == vec_span[0].dist);
	
		//找不到的情况
		m.getSpan(22,22,vec_span);
		assert(0 == vec_span.size());
		m.getSpan(41,49,vec_span);
		assert(0 == vec_span.size());

		//部分找不到，部分跨区间
		m.getSpan(20,21,vec_span);
		assert(0 == vec_span.size());
		m.getSpan(19,21,vec_span);
		assert(1 == vec_span.size());
		assert(11 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.getSpan(19,22,vec_span);
		assert(1 == vec_span.size());
		assert(11 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.getSpan(20,30,vec_span);
		assert(1 == vec_span.size());
		assert(30 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.getSpan(21,30,vec_span);
		assert(1 == vec_span.size());
		assert(30 == vec_span[0].lower && 1 == vec_span[0].dist);


		//跨连续区间
		m.getSpan(0,20,vec_span);
		assert(2 == vec_span.size());
		assert(0 == vec_span[1].lower && 10 == vec_span[1].dist);
		assert(11 == vec_span[0].lower && 10 == vec_span[0].dist);
		m.getSpan(1,20,vec_span);
		assert(2 == vec_span.size());
		assert(0 == vec_span[1].lower && 9 == vec_span[1].dist);
		assert(11 == vec_span[0].lower && 10 == vec_span[0].dist);
		m.getSpan(0,19,vec_span);
		assert(2 == vec_span.size());
		assert(0 == vec_span[1].lower && 10 == vec_span[1].dist);
		assert(11 == vec_span[0].lower && 9 == vec_span[0].dist);
		m.getSpan(10,11,vec_span);
		assert(1 == vec_span.size());
		assert(11 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.getSpan(9,11,vec_span);
		assert(2 == vec_span.size());
		assert(0 == vec_span[1].lower && 1 == vec_span[1].dist);
		assert(11 == vec_span[0].lower && 1 == vec_span[0].dist);

		//跨不连续区间
		m.getSpan(40,50,vec_span);
		assert(1 == vec_span.size());
		assert(50 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.getSpan(39,50,vec_span);
		assert(2 == vec_span.size());
		assert(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.getSpan(39,51,vec_span);
		assert(2 == vec_span.size());
		assert(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 2 == vec_span[0].dist);
		m.getSpan(39,60,vec_span);
		assert(2 == vec_span.size());
		assert(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.getSpan(39,61,vec_span);
		assert(2 == vec_span.size());
		assert(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.getSpan(29,61,vec_span);
		assert(2 == vec_span.size());
		assert(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.getSpan(20,61,vec_span);
		assert(2 == vec_span.size());
		assert(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.getSpan(19,61,vec_span);
		assert(3 == vec_span.size());
		assert(11 == vec_span[2].lower && 1 == vec_span[2].dist);
		assert(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.getSpan(0,61,vec_span);
		assert(4 == vec_span.size());
		assert(0 == vec_span[3].lower && 10 == vec_span[3].dist);
		assert(11 == vec_span[2].lower && 10 == vec_span[2].dist);
		assert(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		assert(50 == vec_span[0].lower && 11 == vec_span[0].dist);

	}	
	void test3()
	{
		MapRegion m;
		assert(m.addRegion(11,20));
		assert(m.addRegion(30,40));
		assert(m.addRegion(50,60));

		MapRegion::VecSpan vec_span;
		m.getSpan(1,3,vec_span);
		assert(0 == vec_span.size());
	}

	namespace
	{
		OverlapRegion g_c_region;

		void initComplexRegion()
		{
			g_c_region.clear();
			g_c_region.addRegion(5,20);
			g_c_region.addRegion(22,25);
			g_c_region.addRegion(15,23);
			g_c_region.addRegion(10,30);
			g_c_region.addRegion(10,20);
			g_c_region.addRegion(15,25);
		}
	}
	//测试点 和 简单区间
	void testOverlapRegion1()
	{
		OverlapRegion::VecLine vec_line;
		OverlapRegion region;
		region.addRegion(10,20);

		region.findOverLap(0, vec_line);
		assert(0 == vec_line.size());
		region.findOverLap(10, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(11, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(20, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(21, vec_line);
		assert(0 == vec_line.size());

		region.erase(10,15);

		region.findOverLap(0, vec_line);
		assert(0 == vec_line.size());
		region.findOverLap(10, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(11, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(20, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(21, vec_line);
		assert(0 == vec_line.size());

		region.erase(10,20);

		region.findOverLap(11, vec_line);
		assert(0 == vec_line.size());

	}
	//测试点 和 复杂区间
	void testOverlapRegion2()
	{
		OverlapRegion::VecLine vec_line;

		g_c_region.findOverLap(4, vec_line);
		assert(0 == vec_line.size());
		g_c_region.findOverLap(5, vec_line);
		assert(1 == vec_line.size());
		assert(5 == vec_line[0].start && 20 == vec_line[0].end);

		g_c_region.findOverLap(9, vec_line);
		assert(1 == vec_line.size());
		g_c_region.findOverLap(10, vec_line);
		assert(3 == vec_line.size());
		g_c_region.findOverLap(14, vec_line);
		assert(3 == vec_line.size());
		g_c_region.findOverLap(15, vec_line);
		assert(5 == vec_line.size());
		g_c_region.findOverLap(19, vec_line);
		assert(5 == vec_line.size());
		g_c_region.findOverLap(20, vec_line);
		assert(5 == vec_line.size());
		g_c_region.findOverLap(21, vec_line);
		assert(3 == vec_line.size());
		g_c_region.findOverLap(22, vec_line);
		assert(4 == vec_line.size());
		g_c_region.findOverLap(23, vec_line);
		assert(4 == vec_line.size());
		g_c_region.findOverLap(25, vec_line);
		assert(3 == vec_line.size());
		g_c_region.findOverLap(26, vec_line);
		assert(1 == vec_line.size());
		g_c_region.findOverLap(30, vec_line);
		assert(1 == vec_line.size());
		g_c_region.findOverLap(31, vec_line);
		assert(0 == vec_line.size());
		g_c_region.findOverLap(331, vec_line);
		assert(0 == vec_line.size());

	}
	//测试区间和简单区间
	void testOverlapRegion3()
	{

		OverlapRegion::VecLine vec_line;
		OverlapRegion region;
		region.addRegion(10,20);
		region.addRegion(22,30);

		region.findOverLap(0, 9, vec_line);
		assert(0 == vec_line.size());
		region.findOverLap(0, 10, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(0, 20, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(0, 21, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(0, 22, vec_line);
		assert(2 == vec_line.size());
		region.findOverLap(0, 29, vec_line);
		assert(2 == vec_line.size());
		region.findOverLap(0, 30, vec_line);
		assert(2 == vec_line.size());
		region.findOverLap(0, 31, vec_line);
		assert(2 == vec_line.size());

		region.findOverLap(31, 0, vec_line);
		assert(0 == vec_line.size());

		region.findOverLap(10, 10, vec_line);
		assert(1 == vec_line.size());
		region.findOverLap(11, 11, vec_line);
		assert(1 == vec_line.size());


	}
	//测试区间和复杂区间
	void testOverlapRegion4()
	{
		OverlapRegion::VecLine vec_line;

		g_c_region.findOverLap(4,3, vec_line);
		assert(0 == vec_line.size());
		g_c_region.findOverLap(4,4, vec_line);
		assert(0 == vec_line.size());
		g_c_region.findOverLap(3,4, vec_line);
		assert(0 == vec_line.size());

		g_c_region.findOverLap(4,5, vec_line);
		assert(1 == vec_line.size());
		g_c_region.findOverLap(4,9, vec_line);
		assert(1 == vec_line.size());
		g_c_region.findOverLap(4,10, vec_line);
		assert(3 == vec_line.size());
		g_c_region.findOverLap(4,15, vec_line);
		assert(5 == vec_line.size());
		g_c_region.findOverLap(4,20, vec_line);
		assert(5 == vec_line.size());
		g_c_region.findOverLap(4,22, vec_line);
		assert(6 == vec_line.size());
		g_c_region.findOverLap(4,23, vec_line);
		assert(6 == vec_line.size());
		g_c_region.findOverLap(4,25, vec_line);
		assert(6 == vec_line.size());
		g_c_region.findOverLap(4,29, vec_line);
		assert(6 == vec_line.size());
		g_c_region.findOverLap(4,30, vec_line);
		assert(6 == vec_line.size());
		g_c_region.findOverLap(4,31, vec_line);
		assert(6 == vec_line.size());

		g_c_region.findOverLap(10,25, vec_line);
		assert(6 == vec_line.size());

		g_c_region.findOverLap(15,23, vec_line);
		assert(6 == vec_line.size());
		g_c_region.findOverLap(20,22, vec_line);
		assert(6 == vec_line.size());
		g_c_region.findOverLap(21,22, vec_line);
		assert(4 == vec_line.size());
		g_c_region.findOverLap(21,21, vec_line);
		assert(3 == vec_line.size());

	}
}//namespace{



void testRegion()
{
	initComplexRegion();
	test1();
	test2();
	test3();
	testOverlapRegion1();
	testOverlapRegion2();
	testOverlapRegion3();
	testOverlapRegion4();
	Info("done");
}