
)模板Trait实现静态多态
template<typename T>
struct My_Traits
{
	static const int V = 0;
};

template<>
struct My_Traits<int>
{
	static const int V = 1;
};

template<>
struct My_Traits<char>
{
	static const int V = 2;
	static void F(char c){};
};


template<typename T>
void ft(T t)
{
	My_Traits<T>::F(t);//对t操作，支持不同类型不一样，间接一层特列化实现。My_Traits<T>::F();. My_Traits<T>可以理解位元函数，不同输入，不同输出。
}

/////////////////////////////////华丽的分割线/////////////////////////////////
)SPINAE 假如有一个特化会导致编译时错误(即出现编译失败)，只要还有别的选择可以被选择，那么就无视这个特化错误而去选择另外的可选选择
//功能，判断类型有某个成员名(C::f)就用方式1处理，没有就用方式2处理
template<class T>
struct HaveFun
{
	template <typename C> static char test(decltype(&C::f)); // C++ 11, 只能推导C为类(非基本类型)，不能推导含有成员f
	//template <typename C> static char test( void (C::*)() ){ return 0; }; //c+=98
	template <typename C>static int32_t test(...);
	static const bool value = (sizeof(test<T>(0)) == 1); //编译时刻，根据T能推导出值.
};

//根据bool参数，特例化其中一个
template<bool c>
struct ft{};
template<>
struct ft<true>
{
	template<class T>
	static void f(T t)
	{
		t.f();//T::f调用成员
	}
};
template<>
struct ft<false>
{
	template<class T>
	static void f(T t)
	{
		//无T::f成员处理
	}
};

template<class T>
void handler(T t)
{
	ft<HaveFun<T>::value>::f(t);
}
void t()
{
	handler(A()); //A为有成员f的类
	handler(3);

}

）获取函数参数数量

template<typename Sig>
struct get_;

template<typename R, typename... Args>
struct get_<R(*)(Args...)> {
	static size_t const value = sizeof...(Args);
};

template<typename Sig>
inline size_t get(Sig) {
	return get_<Sig>::value;
}

void fun(int, int) {}

Info(get(fun)); //获取参数数量
