
)C���Կɱ����
	void Agrs(const char * pattern, ...)
	{
		va_list args;
		va_start(args, pattern);//pattern�����һ��ȷ���Ĳ���

		char para1;
		para1 = va_arg(args, char);
		int para2;
		para2 = va_arg(args, int);

		va_end(args);
	}

)ָ���ڴ�new����, placement new
	type *p = new (place_address)type
	type *p = new (place_address)type(initializers)
	type *p = new (place_address)type[size]
	type *p = new (place_address)type[size] { braced initializer list }

)ģ��ɱ����;
	//֧���޲������
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

)ģ��������;
	template<class Member>
	class RealTimeRankMgr;

	template<class Member>
	class RealTimeRank
	{
		friend class RealTimeRankMgr<Member>;
	}��

)��������ģ��Ϊ��Ԫ
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
		friend bool operator == <> (C1 &a, C1 &b);	
		friend bool ff3 <>(C1 &a, C1 &b);
	protected:
	private:
		int i;
	};

)ģ��������
	׼����һ��C1ģ������
	template<class T>
	struct C1{};
	template<>
	struct C1<char>{};
	���ֲ������л�
	template<class T, bool c = true>
	struct C{};
	template<class T>
	struct C<T, false>{};


)ǰ����++ --���������
	T &operator++() //prefix
	T &operator++(int) //postfix

)��ʼ���б�д�� c++11
	void f4(std::initializer_list<int> c)
	{
		for (auto &v : c)
		{
			printf("%d", v);
		}
	}
	f4({ 1, 2, 3 });

)��Ȼ�ַ�������(ת���ַ���ת��)
char *s = R"offa(
    abc
    ��"
\
\n
end
    )offa";