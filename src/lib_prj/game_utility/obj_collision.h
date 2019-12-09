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
#include "../utility/typedef.h"
#include "../utility/staticReg.h"


typedef const std::type_info *ObjTypeID; //NULL表示无基类
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

