/************************************************************************/
/*         
author:YiliangWu                                                              
brief:简单背包，物品只有一类。
未测试

*/
/************************************************************************/

#pragma once

#include "../../utility/typedef.h"
#include "../../utility/stlBoost.h"
#include <vector>
#include <map>
#include "item.h"

class ItemBase;



//背包：支持动态物品，不记录位置(客户端自动排序),有容量限制,自动堆叠整合。
class MapBag
{
public:

	typedef std::map<uint32, uint32> IdMapNum;
	typedef std::map<uint32, BagGrid> IdMapGrid; //unique id map BagGrid
	typedef IdMapGrid::iterator iterator;
	typedef IdMapGrid::const_iterator const_iterator;
	typedef IdMapGrid::value_type value_type;
	typedef IdMapGrid::key_type key_type;
	typedef IdMapGrid::mapped_type mapped_type;

	MapBag();
	bool enableNewItem(uint32 item_id, uint32 num) const;
	bool newItem(uint32 item_id, uint32 num);
	bool enableDelItem(uint32 item_id, uint32 num) const; 
	bool delItem(uint32 item_id, uint32 num);

	//reset bag max size
	void setGridMaxSize(uint32 size);
	inline uint32 getMaxGridSize() const {return m_max_grid_num;};
	inline uint32 getFreeGridSize() const {return m_max_grid_num-m_grid_num;};

	//批量操作
	//////////////////////////////////////////////////////////////////////////
	bool enableNewItem(const IdMapNum &itemId_m_num) const;
	bool enableDelItem(const IdMapNum &itemId_m_num) const;
	bool newItem(const IdMapNum &itemId_m_num);
	bool delItem(const IdMapNum &itemId_m_num);
	//////////////////////////////////////////////////////////////////////////

	//迭代器操作
	//////////////////////////////////////////////////////////////////////////
	iterator begin();
	iterator end();
	iterator find(uint32 unique_id);
	void erase(iterator it);
	//////////////////////////////////////////////////////////////////////////

	//对物品指针操作，注意分离出的对象，不使用的时候要用alloc().delItem销毁
	//////////////////////////////////////////////////////////////////////////
	bool enableDettach(uint32 unique_id) const;
	ItemBase *detach(uint32 unique_id);
	bool enableAttach(ItemBase *p_item) const;
	bool attach(ItemBase *p_item);
	//////////////////////////////////////////////////////////////////////////

	void read(Memory &out) const;
	void write(const Memory &in);	

private:
	//加入物品，需要格子数
	uint32 addItemNeedGridNum(uint32 item_id, uint32 num) const;
	//物品数量换算成格子数
	uint32 itemNumToGridNum(const ItemBase &item,  uint32 num) const;
	BagGrid *findGridByItemId(uint32 item_id);

	virtual ItemAllocatee &alloc() const=0;

private:
	IdMapGrid m_id_m_grid;
	uint32 m_grid_num;
	uint32 m_max_grid_num;
};


