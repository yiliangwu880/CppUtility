
)C语言可变参数
	void Agrs(const char * pattern, ...)
	{
		va_list args;
		va_start(args, pattern);//pattern是最后一个确定的参数

		char para1;
		para1 = va_arg(args, char);
		int para2;
		para2 = va_arg(args, int);

		va_end(args);
	}

)指定内存new对象, placement new
	type *p = new (place_address)type
	type *p = new (place_address)type(initializers)
	type *p = new (place_address)type[size]
	type *p = new (place_address)type[size] { braced initializer list }

)模板可变参数;
	//支持无参数情况
	inline void AddCallerAgr(ScriptCaller &caller){};
	template<class T>
	void AddCallerAgr(ScriptCaller &caller, T&& t)
	{
		caller += t;
	}

	template<class T, class ... Agrs>
	void AddCallerAgr(ScriptCaller &caller, T &&t, Agrs&& ... agrs)
	{
		caller += t;
		AddCallerAgr(caller, std::forward<Agrs>(agrs)...);
	}

	template<class ReturnType, class ... Agrs>
	ReturnType CallLuaByArgs(const std::string &function_name, Agrs&& ... agrs)
	{
		AddCallerAgr(caller, std::forward<Agrs>(agrs)...);
		return ReturnType();
	}

	)lambda
	[&a, b](..) -> double {}

)模板类申明;
	template<class Member>
	class RealTimeRankMgr;

	template<class Member>
	class RealTimeRank
	{
		friend class RealTimeRankMgr<Member>;
	}；

)申明模板类为友元
template<class T>
	friend class EventCom;

)申明函数模板为友元
	template<class T1>
	class C1;

	template<class T>
	bool operator ==(C1<T> &a, C1<T> &b){

		return b.i == a.i;
	}
	template<class T>
	bool ff3(C1<T> &a, C1<T> &b){

		return b.i == a.i;
	}

	template<class T1>
	class C1
	{
	public:
		friend bool operator == <int> (C1 &a, C1 &b);	
		friend bool ff3 <int>(C1 &a, C1 &b);
	protected:
	private:
		int i;
	};

)模板特例化
	准对上一个C1模板特例
	template<class T>
	struct C1{};
	template<>
	struct C1<char>{};
	部分参数特列化
	template<class T, bool c = true>
	struct AttrsCal{};
	template<class T>
	struct AttrsCal<T, false>{};

	template<typename T>
	inline T auto_hton(T t) //必须加inline 或者 static , extern,不让编译会重定义
	{//不返回，不让它使用
	}
	template<>
	inline uint16_t auto_hton(uint16_t t)
	{
		return htons(t);
	}

)前后置++ --重载运算符
	T &operator++() //prefix
	T &operator++(int) //postfix

)初始化列表写法 c++11
	void f4(std::initializer_list<int> c)
	{
		for (auto &v : c)
		{
			printf("%d", v);
		}
	}
	f4({ 1, 2, 3 });

)自然字符串常量(转义字符不转义)
char *s = R"offa(
    abc
    查"
\
\n
end
    )offa";