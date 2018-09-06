/*
author:yiliangwu
brief: ��̬ע��ʹ������
�ص�:ע�����д��cpp����ط�,����ȥ����ļ�(д�ã���ϣ������޸ĵĿ��ļ�)��ӳ�ʼ������
main������ű�֤��ʼ���ɹ���

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

//ע��ֵ��SetClassName(����ע����), parameter(ע���ֵ)
//���º궨������ڲ�ͬ�ļ�������ã�ʵ�ֲ�ͬcpp�ļ�дע�Ṧ�ܡ�
//����д�ں��������档��֤main����ǰ��ʼ��
#define SET_REG_DEFINE(SetClassName, parameter) \
	namespace{SetClassName::RunReg s_##parameter(parameter);}

#define SET_REG_NAME_DEFINE(SetClassName, obj_name, parameter) \
	namespace{SetClassName::RunReg s_##obj_name(parameter);}

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

//ע��ֵ��SetClassName(����ע����), key(��ֵ�ַ���������"") map_value(ֵ)
//���º궨������ڲ�ͬ�ļ�������ã�ʵ�ֲ�ͬcpp�ļ�дע�Ṧ�ܡ�
#define MAP_REG_DEFINE(SetClassName, key, map_value) \
	namespace{SetClassName::RunReg s_##key(make_pair(SetClassName::key_type(key), map_value));}


//��������
#define MAP_REG_NAME_DEFINE(SetClassName, obj_name, key, map_value) \
	namespace{SetClassName::RunReg obj_name(make_pair(SetClassName::key_type(key), map_value));}


//ע��ֵ��SetClassName(����ע����), str_key(��ֵ�ַ���������"") map_value(ֵ)
#define MAP_REG_NAME_DEFINE_BY_STRING_KEY(SetClassName, str_key, map_value) \
		MAP_REG_NAME_DEFINE(SetClassName, str_key, #str_key, map_value)


