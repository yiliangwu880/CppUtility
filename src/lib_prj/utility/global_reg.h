/*
brief: 全局变量注册：
	使用方法：通过代码任意地方定义全局变量，注册事件队列，使main函数运行前就初始化事件队列。
	目的： 方便写代码，避免固定在某个文件的函数添加注册逻辑。(通常一些库文件，不希望新加功能就修改库文件)
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
	
	//利用静态局部变量，保存注册表，保证第一次调用时初始化容器
	static Vec &container()
	{
		static Vec vec;
		return vec;
	}
};

