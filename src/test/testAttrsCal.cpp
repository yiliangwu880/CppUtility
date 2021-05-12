
#include "../lib_prj/game_utility/attrCal.h"

using namespace std;
using namespace su;



void noMul()
{
	//S12 = (S1+S2)
	//total = RIDE + S12

	Sums sums;
	sums[SubSys::S1] = AttrsCal(Attrs{ {Attr::pAt,2}});
	sums[SubSys::S2] = AttrsCal(Attrs{ {Attr::pAt,8} });
	auto s12 = AttrsCal(sums);


	Sums sums_total;
	sums_total[SubSys::RIDE] = AttrsCal(Attrs{ {Attr::pAtPer,2} });
	sums_total[SubSys::S12] = s12;
	AttrsCal total(sums_total);
	UNIT_INFO("%d", total.GetAttr(Attr::pAt));
	UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
	UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);

	{
		AttrsCal c(Attrs{
			{Attr::pAt,2},
			{Attr::mAtPer,3}
			});
		total.Change(c, { SubSys::S12, SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 3);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAt,-2},
			{Attr::mAtPer,3}
			});
		total.Change(c, { SubSys::S12, SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 6);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 3);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAt,2},
			{Attr::mAtPer,4}
			});
		total.Change(c, { SubSys::S12, SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAt,23},
			{Attr::mAtPer,4}
			});
		UNIT_INFO("NEXT 2 LINE ERROR IS OK");
		total.Change(c, { SubSys::S12 });
		total.Change(c, { SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,2},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,-2},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == -2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,7},
			{Attr::mAtPer,1},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 7);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 5);
	}
}
//没嵌套多层情况
void noMul2()
{
	//total = RIDE + S1+S2

	Sums sums;
	sums[SubSys::S1] = AttrsCal(Attrs{ {Attr::pAt,2} });
	sums[SubSys::S2] = AttrsCal(Attrs{ {Attr::pAt,8} });
	sums[SubSys::RIDE] = AttrsCal(Attrs{ {Attr::pAtPer,2}, {Attr::pAt,0} });
	AttrsCal total(sums);
	UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
	UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);

	{
		AttrsCal c(Attrs{
			{Attr::pAt,2},
			{Attr::mAtPer,3}
			});
		total.Change(c, { SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 3);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAt,-2},
			{Attr::mAtPer,3}
			});
		total.Change(c, { SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 6);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 3);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAt,2},
			{Attr::mAtPer,4}
			});
		total.Change(c, { SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,2},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,-2},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == -2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,7},
			{Attr::mAtPer,1},
			{Attr::pAt,1},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 11);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 7);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 5);
	}
}
//没乘法得情况
UNITTEST(noMul)
{
	noMul();
	noMul2();
}

void Mul()
{
	//S12 = (S1+S2)
	//total = RIDE + S12*0.5

	Sums sums;
	sums[SubSys::S1] = AttrsCal(Attrs{ {Attr::pAt,2 * 2} });
	sums[SubSys::S2] = AttrsCal(Attrs{ {Attr::pAt,8 * 2 } });
	auto s12 = AttrsCal(sums, 0.5);


	Sums sums_total;
	sums_total[SubSys::RIDE] = AttrsCal(Attrs{ {Attr::pAtPer,2 } });
	sums_total[SubSys::S12] = s12;
	AttrsCal total(sums_total);
	UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
	UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);

	{
		AttrsCal c(Attrs{
			{Attr::pAt,2 * 2},
			{Attr::mAtPer,3 * 2}
			});
		total.Change(c, { SubSys::S12, SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 3);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAt,-2 * 2},
			{Attr::mAtPer,3 * 2}
			});
		total.Change(c, { SubSys::S12, SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 6);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 3);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAt,2 * 2},
			{Attr::mAtPer,4 * 2}
			});
		total.Change(c, { SubSys::S12, SubSys::S1 });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,2},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,-2},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == -2);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 4);
	}
	{
		AttrsCal c(Attrs{
			{Attr::pAtPer,7},
			{Attr::mAtPer,1},
			});
		total.Change(c, { SubSys::RIDE });

		UNIT_ASSERT(total.GetAttr(Attr::pAt) == 10);
		UNIT_ASSERT(total.GetAttr(Attr::pAtPer) == 7);
		UNIT_ASSERT(total.GetAttr(Attr::mAtPer) == 5);
	}
}
//没乘法得情况
UNITTEST(Mul)
{
	Mul();
}

UNITTEST(testAttrCal_memory)
{
	return;
	for (;  ; )
	{
		Mul();
	}

}

