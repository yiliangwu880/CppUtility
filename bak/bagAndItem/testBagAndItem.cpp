

#include "bag.h"
#include "item.h"
#include "../../utility/stlBoost.h"
#include "../../utility/misc.h"
#include "../../utility/singleton.h"
#include <assert.h>
#include "stdafx.h"


using namespace wyl;
using namespace std;

namespace
{
	//define class
	//////////////////////////////////////////////////////////////////////////
	class Equipment : public ItemBase
	{
	public:
		Equipment(uint32 id):m_id(id){}
		virtual uint32 id() const{ return m_id;}
		virtual uint32 uniqueId() const {return m_id;}
		virtual uint32 itemId() const {return m_id;}
	private:
		uint32 m_id;
	};	
	
	class PileItem : public ItemBase
	{
	public:
		PileItem(uint32 id, uint32 maxPileNum)
			:m_id(id), m_maxPileNum(maxPileNum){}
		virtual uint32 id() const{ return m_id;}
		virtual uint32 uniqueId() const {return m_id;}
		virtual uint32 itemId() const {return m_id;}
		virtual bool isShare() const{return true;}
		virtual uint32 maxPileNum() const{return m_maxPileNum;}
	private:
		uint32 m_id;
		uint32 m_maxPileNum;
	};

	class ItemMgr : public ItemAllocatee, public SingletonBase<ItemMgr>
	{	
	public:
		ItemMgr():m_unShareItemNum(0),m_shareNum(0){}
		bool findShareItem(uint32 itemID)
		{
			assert(m_shareNum==m_shareItems.size());
			itemIdMapItem::iterator iter=m_shareItems.find(itemID);
			if(iter!=m_shareItems.end())
			{
				return true;
			}
			return false;
		}
		bool isDestoryAllUnShareItem()
		{
			return 0==m_unShareItemNum;
		}
		uint32 unShareItemNum(){return m_unShareItemNum;}
	private:
		virtual ItemBase *doNew(uint32 itemID)
		{
			//0~9 == Equipment, 10~19 == PileItem
			if (itemID < 10)
			{
				m_unShareItemNum++;
				return new Equipment(itemID);
			}
			else
			{
				m_shareNum++;
				return new PileItem(itemID, 3);//max pile = 3
			}
		}
		virtual void doDel(ItemBase *p_item)
		{
			assert(m_unShareItemNum>0);
			m_unShareItemNum--;
			delete p_item;
		}

	private:
		uint32 m_unShareItemNum;
		uint32 m_shareNum;
	};


	typedef VecBag NormalBag;
//test sub function 
	//////////////////////////////////////////////////////////////////////////1
	bool checkIsShareItem(uint32 itemID)
	{
		return ItemMgr::instance().findShareItem(itemID);
	}
	void checkEquipIsNoShareItem()
	{
		for (uint32 i=0; i<10; ++i)
		{
			assert(!checkIsShareItem(i));
		}
	}
	
	#define CREATE_ITEM(bag,id)				bag.newItem(id) //CmdItemCreate::makeInstance(&bag, id).excute()
	#define CREATE_ITEM_NUM(bag,id,num)		bag.newItem(id, num)  //CmdItemCreate::makeInstance(&bag, id, num).excute()
	#define DESTROY_ITEM(bag,id)			bag.delItem(id) //CmdItemDestroy::makeInstance(&bag, id).excute()
	#define DESTROY_ITEM_NUM(bag,id,num)	bag.delItem(id, num) //CmdItemDestroy::makeInstance(&bag, id, num).excute()
	
	
	//加装备，直到包满，再加物品，创建物品失败。
	void test1()
	{
		{
			NormalBag bag(ItemMgr::obj());
			bag.addCapacity(3);
			checkEquipIsNoShareItem();

			BagGrid grid;uint32 index;

			CREATE_ITEM(bag, 1);
			bag.getGridByIndex(0, grid);
			assert(grid.num==1&&grid.p_item->itemId()==1);
			bag.getGridByID(1, grid, index);
			assert(index ==0 && grid.num==1&&grid.p_item->itemId()==1);
			assert(2==bag.emptyGridNum());

			CREATE_ITEM(bag, 2);
			bag.getGridByIndex(1, grid);
			assert(grid.num==1&&grid.p_item->itemId()==2);
			bag.getGridByID(2, grid, index);
			assert(index ==1 && grid.num==1&&grid.p_item->itemId()==2);
			assert(1==bag.emptyGridNum());

			CREATE_ITEM(bag, 2);
			bag.getGridByIndex(2, grid);
			assert(grid.num==1&&grid.p_item->itemId()==2);
			bag.getGridByID(2, grid, index);
			assert(index ==1 && grid.num==1&&grid.p_item->itemId()==2);
			assert(0==bag.emptyGridNum());

			assert(!bag.enableNewItem(2) );
			assert(!bag.enableNewItem(10));
			bag.addCapacity(1);
			assert(bag.newItem(10));
			assert(checkIsShareItem(10));
			assert(!checkIsShareItem(11));
			assert(!bag.enableNewItem(11));
			assert(checkIsShareItem(11));
			checkEquipIsNoShareItem();
		}
		assert(checkIsShareItem(10));
		assert(checkIsShareItem(11));
	}
	
	//加减堆叠物品
	void test2()
	{
		BagGrid grid;
		{
			NormalBag bag(ItemMgr::obj(),3);

			uint32 i = ItemMgr::obj().unShareItemNum();
			CREATE_ITEM_NUM(bag, 10,1);
			CREATE_ITEM_NUM(bag, 10,1);
			bag.getGridByIndex(0, grid);
			assert(grid.num==2);
			bag.getGridByIndex(1, grid);
			assert(grid.num==0&&grid.p_item==NULL);


			DESTROY_ITEM_NUM(bag,10,1);
			bag.getGridByIndex(0, grid);
			assert(grid.num==1);


			CREATE_ITEM_NUM(bag,10,4);
			bag.getGridByIndex(0, grid);
			assert(grid.num==3&&grid.p_item->itemId()==10);
			bag.getGridByIndex(1, grid);
			assert(grid.num==2&&grid.p_item->itemId()==10);

			DESTROY_ITEM_NUM(bag,10,3);
			bag.getGridByIndex(0, grid);
			assert(grid.num==0&&grid.p_item==NULL);
			bag.getGridByIndex(1, grid);
			assert(grid.num==2&&grid.p_item->itemId()==10);

			CREATE_ITEM_NUM(bag,11,5);
			bag.getGridByIndex(0, grid);
			assert(grid.num==3&&grid.p_item->itemId()==11);
			bag.getGridByIndex(2, grid);
			assert(grid.num==2&&grid.p_item->itemId()==11);

			CREATE_ITEM_NUM(bag,11,5);
			CREATE_ITEM_NUM(bag,1,5);
			bag.getGridByIndex(0, grid);
			assert(grid.num==3&&grid.p_item->itemId()==11);
			bag.getGridByIndex(2, grid);
			assert(grid.num==2&&grid.p_item->itemId()==11);
		}

		assert(ItemMgr::instance().isDestoryAllUnShareItem());
	}

	//背包之间物品移动,堆叠物品
	void test4()
	{
		//done in test3
	}
	//背包内部物品移动
	void test5()
	{
		//done in test3
	}
	//背包内部物品移动,堆叠物品
	void test6()
	{
		//done in test3
	}

	//多物品创建, 销毁
	void test7()
	{
		{
			typedef VecBag::ItemIdMapNum MapItemIdNum;
			BagGrid grid;
			NormalBag bag(ItemMgr::obj(),3);
			MapItemIdNum map;
			map.insert(make_pair(1,1));
			map.insert(make_pair(2,1));
			map.insert(make_pair(3,1));
			//CmdItemCreate obj(&bag, map);
			//obj.excute();
			bag.newItem(map);
			//assert(!obj.enableExcute());	
			assert(!bag.enableNewItem(map));

			bag.getGridByIndex(2, grid);
			assert(grid.num==1&&grid.p_item->itemId()==3);	

			//CmdItemDestroy deobj(&bag,map);
		//	deobj.excute();
			bag.delItem(map);
			//assert(!deobj.enableExcute());	
			assert(!bag.enableDelItem(map));
			bag.getGridByIndex(2, grid);
			assert(grid.num==0);	
		}

		//assert(ItemMgr::instance().isDestoryAllUnShareItem());
	}
	//多堆叠物品创建, 销毁
	void test8()
	{
		typedef VecBag::ItemIdMapNum MapItemIdNum;
		{
			BagGrid grid;
			NormalBag bag(ItemMgr::obj(),3);
			MapItemIdNum map;
			map.insert(make_pair(11,3));
			map.insert(make_pair(12,3));
			map.insert(make_pair(13,3));
			//CmdItemCreate obj(&bag, map);
			//obj.excute();
			bag.newItem(map);
			//assert(!obj.enableExcute());	
			assert(!bag.enableNewItem(map));	

			bag.getGridByIndex(2, grid);
			assert(grid.num==3&&grid.p_item->itemId()==13);	
			{
				//CmdItemDestroy deobj(&bag,map);
				//deobj.excute();
				bag.delItem(map);
			//	assert(!deobj.enableExcute());	
				assert(!bag.enableDelItem(map));	
			}

			bag.getGridByIndex(2, grid);
			assert(grid.num==0);	

			//obj.excute();
			bag.newItem(map);
			map.insert(make_pair(14,1));
			{
				//CmdItemDestroy deobj(&bag,map);
				//assert(!deobj.enableExcute());	
				assert(!bag.enableDelItem(map));
			}
		}

		assert(ItemMgr::instance().isDestoryAllUnShareItem());
	}
	//
	void test9()
	{

	}
	//exchange pos
	void test10()
	{
		BagGrid grid;

	}
	//排序
	void test11()
	{
		BagGrid grid;
		{
			NormalBag bag(ItemMgr::obj(),3);

			CREATE_ITEM_NUM(bag, 1,1);
			CREATE_ITEM_NUM(bag, 10,1);	
			CREATE_ITEM_NUM(bag, 2,1);	
			bag.getGridByIndex(1, grid);
			assert(grid.num==1&&grid.p_item->itemId()==10);

			bag.sort();

			bag.getGridByIndex(0, grid);
			assert(grid.num==1&&grid.p_item->itemId()==1);	
			bag.getGridByIndex(1, grid);
			assert(grid.num==1&&grid.p_item->itemId()==2);	
			bag.getGridByIndex(2, grid);
			assert(grid.num==1&&grid.p_item->itemId()==10);	
		}

		assert(ItemMgr::instance().isDestoryAllUnShareItem());
	}
	//存档
	void test12()
	{		
		BagGrid grid;
		{
			NormalBag bag(ItemMgr::obj(),3);

			CREATE_ITEM_NUM(bag, 1,1);
			CREATE_ITEM_NUM(bag, 10,1);	
			CREATE_ITEM_NUM(bag, 2,1);	
			bag.getGridByIndex(1, grid);
			assert(grid.num==1&&grid.p_item->itemId()==10);

			bag.sort();

			bag.getGridByIndex(0, grid);
			assert(grid.num==1&&grid.p_item->itemId()==1);	
			bag.getGridByIndex(1, grid);
			assert(grid.num==1&&grid.p_item->itemId()==2);	
			bag.getGridByIndex(2, grid);
			assert(grid.num==1&&grid.p_item->itemId()==10);	

			char buf[100];
			Memory out((uint8 *)buf, sizeof(buf));
			bag.read(out);
			assert(!out.error());


			Memory in((uint8 *)buf, sizeof(buf));
			NormalBag newBag(ItemMgr::obj(), 1);
			newBag.write(in);
			assert(!in.error());


			newBag.getGridByIndex(0, grid);
			assert(grid.num==1&&grid.p_item->itemId()==1);	
			newBag.getGridByIndex(1, grid);
			assert(grid.num==1&&grid.p_item->itemId()==2);	
			newBag.getGridByIndex(2, grid);
			assert(grid.num==1&&grid.p_item->itemId()==10);	
		}

		assert(ItemMgr::instance().isDestoryAllUnShareItem());

	}
	//背包容量改变
	void test13()
	{
		BagGrid grid;
		{
			NormalBag bag(ItemMgr::obj(), 1);

			CREATE_ITEM_NUM(bag, 1,1);
			bag.getGridByIndex( 0, grid );
			assert(grid.num==1&&grid.p_item->itemId()==1);	
			assert(!CREATE_ITEM_NUM(bag, 1,1));

			assert(bag.capacity()==1);
			bag.addCapacity(1);
			assert(bag.capacity()==2);
			CREATE_ITEM_NUM(bag, 2,1);	
			bag.getGridByIndex( 1, grid );
			assert(grid.num==1&&grid.p_item->itemId()==2);	

		}
		assert(ItemMgr::instance().isDestoryAllUnShareItem());
	}
	//检查背包内部移动
	void test14()
	{
		BagGrid grid;
	
		{
			NormalBag bag(ItemMgr::obj(), 2);

			CREATE_ITEM_NUM(bag,1,1);
			CREATE_ITEM_NUM(bag,2,1);
			{
				//CmdItemExchangePos obj(&bag1, 1, &bag1, 2);
				//assert(!obj.excute());
				assert(!bag.exchangeItemPos(1,  2));
			}
			{
				//CmdItemExchangePos obj(&bag1, 1, &bag1, 0);
				//obj.excute();
				bag.exchangeItemPos(1,  0);
			}
			bag.getGridByIndex( 0, grid );
			assert(grid.num==1&&grid.p_item->itemId()==2);
			bag.getGridByIndex( 1, grid );
			assert(grid.num==1&&grid.p_item->itemId()==1);	
		}
		

		assert(ItemMgr::instance().isDestoryAllUnShareItem());
	}
	//检查内存泄露
	void test15()
	{
	}

	//检查内存泄露
	void test19()
	{
		assert(ItemMgr::instance().isDestoryAllUnShareItem());
	}

	
}//end namespace

void testBagAndItem()
{
	Info("testing Bag");
	test1();
	test2();

	test4();
	test5();
	test6();
	test7();
	test8();
	//test9();
	test10();
	test11();
	test12();
	test13();
	test14();
	test15();
	test19();

	Info("done");
}


