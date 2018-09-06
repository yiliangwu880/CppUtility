/*
获取枚举值字符串
优点：定义方法简洁
使用例子：
{
	#include "enum_str.h"
	#include "枚举定义头文件"

	#define ALL_ENUM_VALUE \
	DEF_ENUM_VALUE(enum_value)\
	DEF_ENUM_VALUE(E22)\

	DEF_ENUM_TYPE(enum_name)
	#undef ALL_ENUM_VALUE


	printf("%s", EnumStr(my_enum_value));
}
*/

#pragma once



template<typename T>
const char *EnumStr(T t)
{
	const void *p = NULL;
	//下面报错表示：获取枚举类型无效。 可以改为 EnumStr<T>() 修复
	int *unknown_type=p; 
	return "unknown type";
}

#define  DEF_ENUM_VALUE(enum_value) case enum_value:return #enum_value;

#define DEF_ENUM_TYPE(E2) \
	template<>\
	const char *EnumStr(E2 t)\
{\
	switch (t)\
{\
	ALL_ENUM_VALUE\
	default:\
	return "unknown";\
	break;\
}\
}

//////////////////////////////////////////




//end file
