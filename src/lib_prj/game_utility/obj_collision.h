/*
ûֱ�Ӹ��ü�ֵ�������вο���ֵ��
δ����
brief:��̬������ײ����,Ӧ�����ӣ�����������ͬ����Ĳ�ͬ�����ɴ����������������߼���һ����
���򣺶����µ���ײ�࣬�����ú�ע�ᴦ������

use example:
//h file
class MyObj : public GameColObj
{
public:
	//��������µĴ�����ӳ�䣬���ӻ��Զ��û����
	static void setHanlde( const ColKey2Handle &handles, GameObjTypeID new_class_id, GameObjTypeID base_id); 
private:
};

//cpp file
	GAME_COL_OBJ_REG(MyObj, GameColObj)
*/

#pragma once
#include "../utility/typedef.h"
#include "../utility/staticReg.h"


typedef const std::type_info *ObjTypeID; //NULL��ʾ�޻���
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

//����1��������������������2����ӽ��Ļ�����
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

