/*
brief: ȫ�ֱ���ע�᣺
	ʹ�÷�����ͨ����������ط�����ȫ�ֱ�����ע���¼����У�ʹmain��������ǰ�ͳ�ʼ���¼����С�
	Ŀ�ģ� ����д���룬����̶���ĳ���ļ��ĺ������ע���߼���(ͨ��һЩ���ļ�����ϣ���¼ӹ��ܾ��޸Ŀ��ļ�)
*/

#pragma once

#include <vector>


#define GLOBAL_REG(type, name, value) namespace{ GlobalReg<type> g_##name(value);}

/*
example:
	main function:
		CONST_FOR_IT(GlobalReg<string>::Vec, GlobalReg<string>::container())
		{
		LOG_DEBUG(it->c_str());
		}
	any file define global:
		namespace{
		GlobalReg<string> c2bb2("6");
		}
		GLOBAL_REG(string, myname, "1");
		
*/
template<typename T>
class GlobalReg
{
public:
	typedef std::vector<T> Vec;
public:
	GlobalReg(const T &t)
	{
		Vec &vec = container();
		vec.push_back(t);
	}
	
	//���þ�̬�ֲ�����������ע�����֤��һ�ε���ʱ��ʼ������
	static Vec &container()
	{
		static Vec vec;
		return vec;
	}
};

