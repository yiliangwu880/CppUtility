

#include "stdafx.h"
#include "../utility/enum_str.h"

#include   <stdio.h>      
#include   <stdio.h>      
#include   <time.h>   

//#ifdef   WIN32      
//#define   LOCALTIME_R(t)     localtime((t))      
//#else      
//#define   LOCALTIME_R(t)     localtime_r((t),   (struct   tm   *)&tmres)      
//#endif   

#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace wyl;

enum E1
{
	E11 = 1,
	E12 = 2,
};
enum E2
{
	E21 = 1,
	E22 = 2,
};

#define ALL_ENUM_VALUE \
	DEF_ENUM_VALUE(E21)\
	DEF_ENUM_VALUE(E22)\

	DEF_ENUM_TYPE(E2) //枚举名
#undef ALL_ENUM_VALUE

#define ALL_ENUM_VALUE \
	DEF_ENUM_VALUE(E11)\
	DEF_ENUM_VALUE(E12)\

	DEF_ENUM_TYPE(E1)

#undef ALL_ENUM_VALUE


namespace
{



	void test1()
	{
		Info(EnumStr(E11));
		Info(EnumStr(E21));
		//Info(EnumStr(1));  //打开必需编译不过才对
	}
}//end namespace

void testEnumName()
{
	Info("testEnumName start");
	test1();
	Info("testEnumName end");
}