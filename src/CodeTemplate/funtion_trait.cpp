方法1：利用std::function
//通过函数名，获取函数参数，返回值 类型 。
//只能适用 1-2个参数的函数。
namespace
{
	void f1(int a, uint64_t b)
	{

	}
	void f2(uint8_t a, uint64_t b)
	{

	}
	int f3(uint8_t a)
	{
		return 0;
	}
	template<class Fun>
	void RegProtoParse3(Fun f)
	{
		//typename std::function<Fun>::argument_type t2 = 231; //ok
		typename std::function<Fun>::first_argument_type t2 = 231; //ok
		typename std::function<Fun>::second_argument_type t1 = 231; //ok
		typename std::function<Fun>::result_type *p = nullptr;

		UNIT_INFO("t2 %d %d", t2, sizeof(t2))
			UNIT_INFO("para3");
	}
}

UNITTEST(fun3)
{
	//RegProtoParse3<decltype(f2)>(f2);
	RegProtoParse3<decltype(f1)>(f1);
	//RegProtoParse3<decltype(f3)>(f3);
}



方法2: 研究发现不能像方法1那样抽象用，待研究。
///////////////////////////////////////////////////
function_traits通过模板特化和可变参数模板来获取函数类型和返回类型
以下源内容来自于《深入应用C++11 代码优化与工程级应用》3.3.6　function_traits

/*
 * function_traits用来获取所有函数语义类型的信息，
 *      (函数类型、返回类型、参数个数和参数的具体类型)。
 *
 * 函数语义类型包括
 *  普通函数
 *  函数指针
 *  function/lambda
 *  成员函数
 *  函数对象
 *
 * 实现function_traits的关键技术
 *  要通过模板特化和可变参数模板来获取函数类型和返回类型。
 *  先定义一个基本的function_traits的模板类：
 *     template<typename T>
 *     struct function_traits;
 *  再通过特化，将返回类型和可变参数模板作为模板参数，就可以获取函数类型、函数返回值和参数的个数了。
 *
 * 如:
 *  int func(int a, string b);
 * ## 获取函数类型
 *      function_traits<decltype(func)>::function_type; // int __cdecl(int, string)
 * # 获取函数返回值
 *      function_traits<decltype(func)>::return_type;   // int
 * # 获取函数的参数个数
 *      function_traits<decltype(func)>::arity;         // 2
 * # 获取函数第一个入参类型
 *      function_traits<decltype(func)>::args<0>::type; // int
 * # 获取函数第二个入参类型
 *      function_traits<decltype(func)>::args<1>::type; // string
 */

#ifndef __FUNCTION_TRAITS_H__
#define __FUNCTION_TRAITS_H__

#include <functional>

 // 原型
	template<typename T>
struct function_traits;

// 普通函数
template<typename ReturnType, typename... Args>
struct function_traits<ReturnType(Args...)>
{
	enum { arity = sizeof...(Args) };
	using return_type = ReturnType;
	using function_type = ReturnType(Args...);
	using stl_function_type = std::function<function_type>;
	using pointer = ReturnType(*)(Args...);

	template<size_t I>
	struct args
	{
		static_assert(I < arity, "index is out of range, index must less than sizeof Args");
		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
	};

	using tuple_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
	using bare_tuple_type = std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...>;
};

// 函数指针
template<typename ReturnType, typename... Args>
struct function_traits<ReturnType(*)(Args...)> : function_traits<ReturnType(Args...)> {};

// std::function
template<typename ReturnType, typename... Args>
struct function_traits<std::function<ReturnType(Args...)>> : function_traits<ReturnType(Args...)> {};

// 成员函数
#define FUNCTION_TRAITS(...)\
template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{};\

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

// 函数对象
template<typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())> {};

template<typename Function>
typename function_traits<Function>::stl_function_type to_function(const Function& lambda)
{
	return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
}

template<typename Function>
typename function_traits<Function>::stl_function_type to_function(Function&& lambda)
{
	return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
}

template<typename Function>
typename function_traits<Function>::pointer to_function_pointer(const Function& lambda)
{
	return static_cast<typename function_traits<Function>::pointer>(lambda);
}

#endif
以下是测试代码

#include <iostream>
#include <string>
#include "function_traits.hpp"
using namespace std;

int func(int a, string b);

template<typename T>
void printType()
{
	cout << typeid(T).name() << endl;
}

float(*cast_func)(int, int, int, int);
float free_function(const string& a, int b)
{
	return (float)a.size() / b;
}
struct AA
{
	int f(int a, int b)volatile { return a + b; }
	int operator()(int)const { return 0; }
};

void test_function_traits();

int main()
{
	{
		// 获取函数类型
		function_traits<decltype(func)>::function_type;     // int __cdecl(int, string)
		// 获取函数返回值  
		function_traits<decltype(func)>::return_type;       // int
		// 获取函数的参数个数    
		function_traits<decltype(func)>::arity;             // 2
		// 获取函数第一个入参类型
		function_traits<decltype(func)>::args<0>::type;     // int
		// 获取函数第二个入参类型
		function_traits<decltype(func)>::args<1>::type;     // string

		cout << typeid(function_traits<decltype(func)>::function_type).name() << endl;
		cout << typeid(function_traits<decltype(func)>::return_type).name() << endl;
	}
	{
		test_function_traits();
	}
	return 0;
}

void test_function_traits()
{
	cout << "----------- 2 ----------" << endl;
	std::function<int(int)> f = [](int a) {return a; };
	printType<function_traits<std::function<int(int)>>::function_type>();
	printType<function_traits<std::function<int(int)>>::args<0>::type>();
	printType<function_traits<decltype(f)>::function_type>();
	printType<function_traits<decltype(free_function)>::function_type>();
	printType<function_traits<decltype(cast_func)>::function_type>();
	printType<function_traits<AA>::function_type>();
	using T = decltype(&AA::f);
	printType<T>();
	printType<function_traits<decltype(&AA::f)>::function_type>();
	static_assert(std::is_same<function_traits<decltype(f)>::return_type, int>::value, "");
}

int func(int a, string b)
{
	return 0;
}