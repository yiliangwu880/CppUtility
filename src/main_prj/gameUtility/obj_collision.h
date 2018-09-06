/*
δ����
brief:��̬������ײ����,Ӧ�����ӣ�����������ͬ����ľ���ɴ����������������߼���һ����
���򣺶����µ���ײ�࣬�����ú�ע�ᴦ������

use example:
//h file
class MyObj : public GameColObj
{
public:
	static void setHandle(ColKeyMapHandle &handles); //��������µĴ�����ӳ�䣬���ӻ��Զ��û����
private:
};

//cpp file
	GAME_COL_OBJ_REG(MyObj, GameColObj)
*/

#pragma once
#include "../utility/typedef.h"
#include "../utility/staticReg.h"


typedef const std::type_info *GameObjTypeID; //NULL��ʾ�޻���
class GameColObj;
typedef std::pair<GameObjTypeID, GameObjTypeID> ColKey;
typedef void (*HandleCol)(GameColObj &left, GameColObj &right);
typedef std::map<ColKey, HandleCol> ColKeyMapHandle;

class GameColObj
{
public:
	static void regHandle( const ColKeyMapHandle &handles, GameObjTypeID new_class_id, GameObjTypeID base_id);
protected:
private:
};

class MyObj : public GameColObj
{
public:
	static void setHanlde(ColKeyMapHandle &handles);
private:
};

//����1��������������������2����ӽ��Ļ�����
#define GAME_COL_OBJ_REG(class_name, close_base_name)	\
namespace{\
struct GameObjReg##class_name\
{\
	GameObjReg##class_name()\
	{\
		ColKeyMapHandle handles;\
		class_name::setHanlde(handles);\
		GameColObj::regHandle(handles, &typeid(class_name), &typeid(close_base_name));\
	}\
};\
GameObjReg##class_name s_reg_##class_name;\
}\

REG_MAP_NAME_DECLARE(RegColHandle, ColKey, HandleCol)

