//auto

//decltype
class {
int x = 3;
decltype(x) y = x;

//auto decltype 配合定义返回值
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
		//[] 不截取任何变量
		//[&]截取外部作用域中所有变量，并作为引用在函数体中使用
		//[=] 截取外部作用域中所有变量，并拷贝一份在函数体中使用
		//[=, &foo]   截取外部作用域中所有变量，并拷贝一份在函数体中使用，但是对foo变量使用引用
		//[bar]   截取bar变量并且拷贝一份在函数体重使用，同时不截取其他变量
		//[this]            截取当前类中的this指针。如果已经使用了&或者 = 就默认添加此选项。
}

//tuple
class
{
	auto t2 = make_tuple(1, 2.0, "C++ 11", { 1, 0, 2 });
};

//typename ...  变长参数的模板
class 
{
		//递归读取变长参数
	template<typename head, typename... tail>
	void Print(Head head, typename... tail) {
		cout << head << endl;
		Print(tail...);
	}
};

//初始化方法
class
{
	vector<int> iv{ 1, 2, 3 };
	map<int, string>{ {1, "a"}, { 2, "b" }};
	string str{ "Hello World" };
};

enum MyEnum: char  //指定字节
{

};

enum class MyClass
{

};

