/*
brief: 动态对象的流化 功能
*/

#include "test_obj_stream.h"


//定义创建对象函数
NEW_STREAM_OBJ_DEFINE(SubStreamObj_1);
NEW_STREAM_OBJ_DEFINE(SubStreamObj_2);
NEW_STREAM_OBJ_DEFINE(SubStreamObj_3);

bool SubStreamObj_1::doRead( Memory &out ) const
{
	out << m_name;
	return out;
}

bool SubStreamObj_1::doWrite( const Memory &in )
{
	in >> m_name;
	return in;
}
bool SubStreamObj_2::doRead( Memory &out ) const
{
	out << m_name;
	return out;
}

bool SubStreamObj_2::doWrite( const Memory &in )
{
	in >> m_name;
	return in;

}
bool SubStreamObj_3::doRead( Memory &out ) const
{
	out << m_o3_name;
	SubStreamObj_2::doRead(out);
	return out;
}

bool SubStreamObj_3::doWrite( const Memory &in )
{
	in >> m_o3_name;
	SubStreamObj_2::doWrite(in);
	return in;
}

void test_obj_stream()
{
	uint8 buf[1000];
	const Memory in(buf, 1000);
	Memory out(buf, 1000);
	SubStreamObj_1 o1;
	SubStreamObj_2 o2;
	SubStreamObj_3 o3;
	o1.m_name = "o1";
	o2.m_name = "o2";
	o3.m_name = "o3";
	o3.m_o3_name = "m_o3_name";

	assert(o1.read(out));
	assert(o3.read(out));
	assert(o2.read(out));

	//read
	{
		StreamObj *p1 = StreamObj::newAndWrite(in);
		assert(p1);
		StreamObj *p3 = StreamObj::newAndWrite(in);
		assert(p3);
		StreamObj *p2 = StreamObj::newAndWrite(in);
		assert(p2);
		SubStreamObj_1 *pc1 = dynamic_cast<SubStreamObj_1*>(p1);
		assert(pc1!=NULL);
		SubStreamObj_3 *pc3 = dynamic_cast<SubStreamObj_3*>(p3);
		assert(pc3!=NULL);
		SubStreamObj_2 *pc2 = dynamic_cast<SubStreamObj_2*>(p2);
		assert(pc2!=NULL);

		assert(pc1->m_name == "o1");
		assert(pc3->m_name == "o3");
		assert(pc2->m_name == "o2");

		assert(pc3->m_o3_name == "m_o3_name");

		delete p1;
		delete p2;
		delete p3;
	}
	Info("done");
}


