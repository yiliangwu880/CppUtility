
)ģ��Traitʵ�־�̬��̬
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
	My_Traits<T>::F(t);//��t������֧�ֲ�ͬ���Ͳ�һ�������һ�����л�ʵ�֡�My_Traits<T>::F();. My_Traits<T>�������λԪ��������ͬ���룬��ͬ�����
}

/////////////////////////////////�����ķָ���/////////////////////////////////
)SPINAE ������һ���ػ��ᵼ�±���ʱ����(�����ֱ���ʧ��)��ֻҪ���б��ѡ����Ա�ѡ����ô����������ػ������ȥѡ������Ŀ�ѡѡ��
//���ܣ��ж�������ĳ����Ա��(C::f)���÷�ʽ1����û�о��÷�ʽ2����
template<class T>
struct HaveFun
{
	template <typename C> static char test(decltype(&C::f)); // C++ 11, ֻ���Ƶ�CΪ��(�ǻ�������)�������Ƶ����г�Աf
	//template <typename C> static char test( void (C::*)() ){ return 0; }; //c+=98
	template <typename C>static int32_t test(...);
	static const bool value = (sizeof(test<T>(0)) == 1); //����ʱ�̣�����T���Ƶ���ֵ.
};

//����bool����������������һ��
template<bool c>
struct ft{};
template<>
struct ft<true>
{
	template<class T>
	static void f(T t)
	{
		t.f();//T::f���ó�Ա
	}
};
template<>
struct ft<false>
{
	template<class T>
	static void f(T t)
	{
		//��T::f��Ա����
	}
};

template<class T>
void handler(T t)
{
	ft<HaveFun<T>::value>::f(t);
}
void t()
{
	handler(A()); //AΪ�г�Աf����
	handler(3);

}

����ȡ������������

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

Info(get(fun)); //��ȡ��������
