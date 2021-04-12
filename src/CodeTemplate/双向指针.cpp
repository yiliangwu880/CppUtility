//适用场景：A,B对象互相指向， 一对一关系。又需要自动释放
template <typename Owner, typename Other>
class DoublePoint
{
	friend class DoublePoint<Other, Owner>;
	Other *othter = nullptr;
	DoublePoint<Other, Owner> *otherProxy = nullptr;
	Owner &owner;
public:
	DoublePoint(Owner &in_owner)
		:owner(in_owner)
	{
	}
	~DoublePoint()
	{
		Detach();
	}
	Other * operator->() const
	{
		return othter;
	}
	explicit operator bool() const noexcept
	{
		return othter != nullptr;
	}


	void Attach(DoublePoint<Other, Owner> &bp)
	{
		if (nullptr != othter)
		{
			Detach();
		}
		othter = &(bp.owner);
		otherProxy = &bp;

		bp.othter = &owner;
		bp.otherProxy = this;
	}
	void Detach()
	{
		if (nullptr != otherProxy)
		{
			otherProxy->othter = nullptr;
			otherProxy->otherProxy = nullptr;
		}
		othter = nullptr;
		otherProxy = nullptr;
	}
private:
};


template<typename _Tp, typename _Dp>
inline bool
operator==(nullptr_t, const DoublePoint<_Tp, _Dp>& __x) noexcept
{
	return !__x;
}
template<typename _Tp, typename _Dp>
inline bool
operator==(const DoublePoint<_Tp, _Dp>& __x, nullptr_t) noexcept
{
	return !__x;
}
template<typename _Tp, typename _Dp>
inline bool
operator!=(nullptr_t, const DoublePoint<_Tp, _Dp>& __x) noexcept
{
	return bool(__x);
}
template<typename _Tp, typename _Dp>
inline bool
operator!=(const DoublePoint<_Tp, _Dp>& __x, nullptr_t) noexcept
{
	return bool(__x);
}


//////////////////////////////////////

namespace su
{


	class B;
	class A
	{
	public:
		int i = 1;
		DoublePoint<A, B> pB;
		A()
			:pB(*this)
		{

		}
		~A()
		{}

	private:

	};

	class B
	{
	public:
		int i = 2;
		DoublePoint<B, A> pA;
		B()
			:pA(*this)
		{

		}
		~B() {}

	private:

	};
}
UNITTEST(TestPointProxy)
{
	A *a = new A();
	B *b = new B();
	B *b2 = new B();
	b2->i = 3;
	UNIT_ASSERT(nullptr == a->pB);
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!a->pB);
	UNIT_ASSERT(!b->pA);
	a->pB.Attach(b->pA);
	UNIT_ASSERT(nullptr != a->pB);
	UNIT_ASSERT(b->pA != nullptr);
	UNIT_ASSERT(a->pB);
	UNIT_ASSERT(b->pA);

	UNIT_INFO("a->bp->i =%d", a->pB->i);
	UNIT_ASSERT(a->pB->i == 2);
	UNIT_ASSERT(b->pA->i == 1);
	////////////////////////
	a->pB.Detach();
	UNIT_ASSERT(nullptr == a->pB);
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!a->pB);
	UNIT_ASSERT(!b->pA);

	b2->pA.Attach(a->pB);
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!b->pA);

	UNIT_ASSERT(nullptr != a->pB);
	UNIT_ASSERT(b2->pA != nullptr);
	UNIT_ASSERT(a->pB);
	UNIT_ASSERT(b2->pA);
	UNIT_ASSERT(a->pB->i == 3);
	UNIT_ASSERT(b2->pA->i == 1);

	a->pB.Detach();
	UNIT_ASSERT(nullptr == a->pB);
	UNIT_ASSERT(b2->pA == nullptr);
	UNIT_ASSERT(!a->pB);
	UNIT_ASSERT(!b2->pA);
	//////////////测试 自动detach/////////////

	a->pB.Attach(b->pA);
	UNIT_ASSERT(nullptr != a->pB);
	UNIT_ASSERT(b->pA != nullptr);
	UNIT_ASSERT(a->pB);
	UNIT_ASSERT(b->pA);
	UNIT_ASSERT(a->pB->i == 2);
	UNIT_ASSERT(b->pA->i == 1);
	delete a;
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!b->pA);
	delete b;


	UNIT_INFO("===================end ==========");
}