//auto

//decltype
class {
int x = 3;
decltype(x) y = x;

//auto decltype ��϶��巵��ֵ
template <typename Creator>
auto processProduct(const Creator& creator) -> decltype(creator.makeObject()) {
	auto val = creator.makeObject();
	return val;
}
}

//for (auto p : m){ ..}

//lambda
class{
	[](int &x)->int {return x * 3;}  //descript [extern args](Fun args)->return type{body}
		//[] ����ȡ�κα���
		//[&]��ȡ�ⲿ�����������б���������Ϊ�����ں�������ʹ��
		//[=] ��ȡ�ⲿ�����������б�����������һ���ں�������ʹ��
		//[=, &foo]   ��ȡ�ⲿ�����������б�����������һ���ں�������ʹ�ã����Ƕ�foo����ʹ������
		//[bar]   ��ȡbar�������ҿ���һ���ں�������ʹ�ã�ͬʱ����ȡ��������
		//[this]            ��ȡ��ǰ���е�thisָ�롣����Ѿ�ʹ����&���� = ��Ĭ����Ӵ�ѡ�
}

//tuple
class
{
	auto t2 = make_tuple(1, 2.0, "C++ 11", { 1, 0, 2 });
};

//typename ...  �䳤������ģ��
class 
{
		//�ݹ��ȡ�䳤����
	template<typename head, typename... tail>
	void Print(Head head, typename... tail) {
		cout << head << endl;
		Print(tail...);
	}
};

//��ʼ������
class
{
	vector<int> iv{ 1, 2, 3 };
	map<int, string>{ {1, "a"}, { 2, "b" }};
	string str{ "Hello World" };
};

enum MyEnum: char  //ָ���ֽ�
{

};

enum class MyClass
{

};

