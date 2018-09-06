/************************************************************************/
/*                                                                      
brief:
author:YiliangWu 
item base code
*/
/************************************************************************/

#pragma once

#include "../../utility/typedef.h"
#include "../../utility/memoryStream.h"
#include <vector>



class ItemBase
{
public:
	
	//物品id,可以重复
	virtual uint32 itemId() const = 0;
	virtual uint32 uniqueId() const = 0;
	virtual bool isShare()const {return false;}; 
	virtual uint32 maxPileNum() const {return 1;};
	virtual bool comparer(ItemBase *other) const{return itemId()<other->itemId();};

	virtual void serialize(Memory &out) const{};
	virtual void unserialize(const Memory &in){};
private:
};

//由于存在共享物品对象，派生类必须为singleton
class ItemAllocatee
{	
public:
	ItemBase *newItem(uint32 itemID);
	void delItem(ItemBase *pItem);

private:
	virtual ItemBase *doNew(uint32 itemID)=0;
	virtual void doDel(ItemBase *pItem)=0;

protected: 
	typedef std::map<uint32, ItemBase *> itemIdMapItem;
	//保存共享物品，一般不需要销毁
	itemIdMapItem m_shareItems; 
};

struct BagGrid
{
	BagGrid(uint32 _num, ItemBase *_p_item)
		:num(_num)
		,p_item(_p_item)
	{}
	BagGrid():num(0), p_item(NULL){};
	uint32 num;
	ItemBase *p_item;
};