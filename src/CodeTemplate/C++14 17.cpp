//lamba
class
{
	auto lambda = [](auto x, auto y) {return x + y;};
};

//constexpr
class
{
	if constexpr(3 == 4)
	{

	}
	if constexpr (std::is_integer ...) { //integerstuff }
	else if constexpr (std::is_floating_point ...) { //floatingpointstuff }
	else { // NaN ;) }

};

//auto 非类型模板参数的 
class
{
	template <typename T, typename U>
	auto add(T t, U u) {
		return t + u;
	}


	template <auto value> void foo() {
		return;
	}
};

//std::variant<>
class
{
	std::variant<int, float> v, w;
	v = 12;
	int i = get<int>(v);
	w = get<int>(v);
	w = get<0>(v); // same effect as the previous line
	w = v; // same effect as the previous line

	get<double>(v); // ill formed
	get<3>(v); // ill formed

	try {
		get<float>(w); // will throw.
	}
	catch (bad_variant_access&) {}
};

//结构化绑定(Structured bindings)
class
{

	std::tuple<int, double, std::string> f() {
		return std::make_tuple(1, 2.3, "456");
	}
	auto[x, y, z] = f(); // x,y,z 分别被推导为int,double,std::string
};

//变量声明的强化
class
{

	if (auto p = m.try_emplace(key, value); !p.second) {
		//...
	}
	else {
		//...
	}
};