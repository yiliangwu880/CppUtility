/*
author:yiliangwu
brief: 静态注册使用例子
特点:注册可以写在cpp任意地方,避免去别的文件(写好，不希望随便修改的库文件)添加初始化代码
main函数后才保证初始化成功。

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

//注册值，SetClassName(单件注册类), key(键值字符串，不用"") map_value(值)
//以下宏定义可以在不同文件里面调用，实现不同cpp文件写注册功能。
#define MAP_REG_DEFINE(SetClassName, key, map_value) \
	namespace{SetClassName::RunReg s_##key(make_pair(SetClassName::key_type(key), map_value));}


//不建议用
#define MAP_REG_NAME_DEFINE(SetClassName, obj_name, key, map_value) \
	namespace{SetClassName::RunReg obj_name(make_pair(SetClassName::key_type(key), map_value));}


//注册值，SetClassName(单件注册类), str_key(键值字符串，不用"") map_value(值)
#define MAP_REG_NAME_DEFINE_BY_STRING_KEY(SetClassName, str_key, map_value) \
		MAP_REG_NAME_DEFINE(SetClassName, str_key, #str_key, map_value)


