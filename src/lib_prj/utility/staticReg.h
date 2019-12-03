/*
author:yiliangwu
brief: 静态注册使用例子
特点:
	注册可以写在cpp任意地方,免去到处翻代码文件添加声明或者定义
	main函数后才保证初始化成功。

缺点：实现原理不好理解

use example:
//in h file
REG_MAP_NAME_DECLARE(MapName2, string, int)

//in any cpp file
MAP_REG_NAME_DEFINE_BY_STRING_KEY(MapName2, a, 1); 
MAP_REG_NAME_DEFINE_BY_STRING_KEY(MapName2, abc, 2);

*/
#pragma once


#include <set>
#include <map>
#include <vector>
#include <string>

//h文件定义唯一单件注册类 SET
#define REG_SET_NAME_DECLARE(SetClassName, ValueType)\
class SetClassName : public std::set<ValueType>\
{\
public:\
	static SetClassName &obj()\
	{\
		static SetClassName d;\
		return d;\
	}\
	struct RunReg \
		{\
		RunReg(ValueType para)\
			{ \
			SetClassName::obj().insert(para).second; \
			};\
		}; \
private:\
	SetClassName(){};\
};

//注册值，SetClassName(单件注册类), parameter(注册的值)
//以下宏定义可以在不同文件里面调用，实现不同cpp文件写注册功能。
//不能写在函数体里面。保证main函数前初始化
#define SET_REG_DEFINE(SetClassName, parameter) \
	namespace{SetClassName::RunReg s_##parameter(parameter);}

#define SET_REG_NAME_DEFINE(SetClassName, obj_name, parameter) \
	namespace{SetClassName::RunReg s_##obj_name(parameter);}

//h文件定义唯一单件注册类 MAP
#define REG_MAP_NAME_DECLARE(SetClassName, KeyType, MapType)\
class SetClassName : public std::map<KeyType, MapType>\
{\
public:\
	static SetClassName &obj()\
	{\
	static SetClassName d; \
	return d; \
	}\
	struct RunReg \
	{\
		RunReg(const std::pair<KeyType, MapType> &para)\
		{ \
			SetClassName::obj().insert(para).second; \
		};\
	}; \
private:\
	SetClassName(){};\
};

//多一层，避免__LINE__直接变化成字符串"__LINE__"
#define MAG_REG_LINENAME_CAT(name, line) name##line
#define MAG_REG_LINENAME(name, line) MAG_REG_LINENAME_CAT(name, line) 

#define MAP_REG_NAME_DEFINE(SetClassName, obj_name, key, map_value) \
	namespace{SetClassName::RunReg MAG_REG_LINENAME(obj_name, __LINE__)(make_pair(SetClassName::key_type(key), SetClassName::value_type::second_type(map_value)));}

//注册值，SetClassName(单件注册类), key(键值字符串，不用"") map_value(值)
#define MAP_REG_DEFINE(SetClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(SetClassName, key, #key, map_value)





