


#include "memoryStream.h"
#include "misc.h"
#include <assert.h>
#include "mem_pool.h"

using namespace std;
using namespace wyl;
namespace{

	struct T1
	{
		uint8 a;
		uint32 b;
	};

	struct T_OVER_MAX
	{
		uint8 a[1000];
	};
	uint8 buf[100];

	void test1()
	{

		Memory out(buf, ArrayLen(buf));
		const Memory in(buf, ArrayLen(buf));
		T1 t1;
		t1.a = 1;t1.b=2;
		out<<t1;

		T1 t2;
		in>>t2;

		assert(t2.a==1 && t2.b==2);
		assert(!out.error());
		assert(!in.error());
		assert(sizeof(T1)==out.offset());
		assert(sizeof(T1)==in.offset());


	//	out << t2; //compile error
	}
	//测试溢出
	void test2() 
	{

		Memory out(buf, ArrayLen(buf));
		const Memory in(buf, ArrayLen(buf));
		T_OVER_MAX t1;
		out<<t1;

		assert(out.error());

		T_OVER_MAX t2;
		in>>t2;

		assert(in.error());assert(!in);
		assert(0==out.offset());
		assert(0==in.offset());
	}


	struct T
	{
		int a,b;
	}t1, t2;

	void test3()
	{

		Memory out(buf, ArrayLen(buf));
		const Memory in(buf, ArrayLen(buf));
		std::string s="aabc";
		std::string outS;
		t1.a = 1;
		t1.b = 2;
		t2.a = 0;
		t2.b = 0;

		out<<t1<<s;
		in >> t2 >>outS;
		assert(t1.a==t2.a);
		assert(t1.b==t2.b);
		assert(outS==s);
		assert(out);

	}

	template<typename T>
	void testLineCtn(T &c_in, T& c_out)
	{
		Memory out(buf, ArrayLen(buf));
		const Memory in(buf, ArrayLen(buf));

		c_in.push_back(1);
		c_in.push_back(2);
		c_in.push_back(3);
		out << 11;
		out << c_in;
		out << 22;
		int i2, i1;
		in >> i1;
		in >> c_out;
		in >> i2;
		assert(c_in==c_out);
		assert(11 == i1);
		assert(22 == i2);
	}
	void test4()
	{

		{
			vector<int> c_in, c_out;
			testLineCtn(c_in, c_out);
		}

		//test list
		{
			list<int> c_in, c_out;
			testLineCtn(c_in, c_out);
		}
		{
			deque<int> c_in, c_out;
			testLineCtn(c_in, c_out);
		}		
		//test map
		{
			Memory out(buf, ArrayLen(buf));
			const Memory in(buf, ArrayLen(buf));
			map<int,int> c_in, c_out;
			c_in[1]=11;
			c_in[2]=22;
			c_in[3]=33;

			out << c_in;
			in >> c_out;  

			
			assert (c_out == c_in);
		}	
		//test set
		{
			Memory out(buf, ArrayLen(buf));
			const Memory in(buf, ArrayLen(buf));
			set<int> c_in, c_out;
			c_in.insert(1);
			c_in.insert(2);
			c_in.insert(3);

			out << c_in;
			in >> c_out;  

			assert (c_out == c_in);
		}	
	}
}



class MemObj
{
public:
	NEWCLASS(MemObj);
	DELETECLASS(MemObj);


	MemObj()
	{
		Info("create");
	}
	~MemObj()
	{
		Info("destroy");
	}


private:
	int i;
};

MEM_POOL_SET_SIZE(MemObj, 3)

static void testMemPool()
{
	MemObj *p = new MemObj;
	delete p;
	//delete p; //调用会报错
	//p++;
	//delete p;//调用会报错
}

void testMemoryStream()
{
	test1();
	test2();
	test3();
	test4();
	testMemPool();
	Info("testMemoryStream done");
}