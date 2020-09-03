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
#include "../../svr_util/include/typedef.h"
//#include "../utility/staticReg.h"

/*
author:yiliangwu
brief: ��̬ע��ʹ������
�ص�:
	ע�����д��cpp����ط�,��ȥ�����������ļ�����������߶���
	main������ű�֤��ʼ���ɹ���

ȱ�㣺ʵ��ԭ�������

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

//------------------------------------set ��̬ע��----------------------------------------------
//h�ļ�����Ψһ����ע���� SET
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


//���º궨������ڲ�ͬ�ļ�������ã�ʵ�ֲ�ͬcpp�ļ�дע�Ṧ�ܡ�
//@SetClassName(����ע����), 
//@value(ע���ֵ)
#define SET_REG_DEFINE(SetClassName, value) \
	namespace{SetClassName::RunReg SET_REG_LINENAME(s##value, __LINE__)(value);}

#define SET_REG_NAME_DEFINE(SetClassName, obj_name, parameter) \
	namespace{SetClassName::RunReg s_##obj_name(parameter);}

//------------------------------------map ��̬ע��----------------------------------------------
//h�ļ�����Ψһ����ע���� MAP
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

//��һ�㣬����__LINE__ֱ�ӱ仯���ַ���"__LINE__"
#define MAG_REG_LINENAME_CAT(name, line) name##line
#define MAG_REG_LINENAME(name, line) MAG_REG_LINENAME_CAT(name, line) 

#define MAP_REG_NAME_DEFINE(SetClassName, obj_name, key, map_value) \
	namespace{SetClassName::RunReg MAG_REG_LINENAME(obj_name, __LINE__)(make_pair(SetClassName::key_type(key), SetClassName::value_type::second_type(map_value)));}

//ע��ֵ��SetClassName(����ע����), key(��ֵ�ַ���������"") map_value(ֵ)
#define MAP_REG_DEFINE(SetClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(SetClassName, key, #key, map_value)







typedef const std::type_cfg *ObjTypeID; //NULL��ʾ�޻���
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

