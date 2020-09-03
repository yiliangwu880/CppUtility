/*
没直接复用价值，但很有参考价值，
未测试
brief:动态对象碰撞处理,应用例子，各种来自相同基类的不同派生飞船互相碰到，处理逻辑不一样。
规则：定义新的碰撞类，必须用宏注册处理函数。

use example:
//h file
class MyObj : public GameColObj
{
public:
	//里面加入新的处理函数映射，不加会自动用基类的
	static void setHanlde( const ColKey2Handle &handles, GameObjTypeID new_class_id, GameObjTypeID base_id); 
private:
};

//cpp file
	GAME_COL_OBJ_REG(MyObj, GameColObj)
*/

#pragma once
#include "../../svr_util/include/typedef.h"
//#include "../utility/staticReg.h"

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
MAP_REG_DEFINE(MapName2, a, 1);
MAP_REG_DEFINE(MapName2, abc, 2);

*/


#include <set>
#include <map>
#include <vector>
#include <string>

//------------------------------------set 静态注册----------------------------------------------
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


#define SET_REG_LINENAME_CAT(name, line) name##line
#define SET_REG_LINENAME(name, line) SET_REG_LINENAME_CAT(name, line) 


//以下宏定义可以在不同文件里面调用，实现不同cpp文件写注册功能。
//@SetClassName(单件注册类), 
//@value(注册的值)
#define SET_REG_DEFINE(SetClassName, value) \
	namespace{SetClassName::RunReg SET_REG_LINENAME(s##value, __LINE__)(value);}

#define SET_REG_NAME_DEFINE(SetClassName, obj_name, parameter) \
	namespace{SetClassName::RunReg s_##obj_name(parameter);}

//------------------------------------map 静态注册----------------------------------------------
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







typedef const std::type_cfg *ObjTypeID; //NULL表示无基类
class GameColObj;
typedef std::pair<ObjTypeID, ObjTypeID> ColKey;
typedef void (*HandleCol)(GameColObj &left, GameColObj &right);
typedef std::map<ColKey, HandleCol> ColKey2Handle;

class GameColObj
{
public:
	static void regHandle( const ColKey2Handle &handles, ObjTypeID new_class_id, ObjTypeID base_id);
protected:
private:
};

class MyObj : public GameColObj
{
public:
	static void setHanlde(ColKey2Handle &handles);
private:
};

//参数1，创建的新类名，参数2，最接近的基类名
#define GAME_COL_OBJ_REG(class_name, close_base_name)	\
namespace{\
struct GameObjReg##class_name\
{\
	GameObjReg##class_name()\
	{\
		ColKey2Handle handles;\
		class_name::setHanlde(handles);\
		GameColObj::regHandle(handles, &typeid(class_name), &typeid(close_base_name));\
	}\
};\
GameObjReg##class_name s_reg_##class_name;\
}\

REG_MAP_NAME_DECLARE(RegColHandle, ColKey, HandleCol)

