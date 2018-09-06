/*
��ȡö��ֵ�ַ���
�ŵ㣺���巽�����
ʹ�����ӣ�
{
	#include "enum_str.h"
	#include "ö�ٶ���ͷ�ļ�"

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
	//���汨���ʾ����ȡö��������Ч�� ���Ը�Ϊ EnumStr<T>() �޸�
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
