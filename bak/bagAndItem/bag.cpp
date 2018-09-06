

#include "bag.h"
#include "item.h"
#include <assert.h>
#include <algorithm>
#include "../../utility/misc.h"

using namespace std;

VecBag::VecBag( ItemAllocatee &alloc )
:m_alloc(alloc)
{
}

VecBag::VecBag( ItemAllocatee &alloc, uint32 capacity )
:m_alloc(alloc)
{
	m_grids.resize(capacity);
}

VecBag::~VecBag()
{
	CONST_FOR_IT(VecGrid, m_grids)
	{
		if (NULL != it->p_item)
		{
			m_alloc.delItem(it->p_item);
		}
	}
}

void VecBag::sort()
{
	struct Pred 
	{
		bool operator()(const BagGrid &left, const BagGrid &right)
		{
			//空格子为最大，排后面
			if (NULL == left.p_item && NULL == right.p_item)
			{
				return false;
			}
			if (NULL == left.p_item )
			{
				return false;
			}
			if (NULL == right.p_item )
			{
				return true;
			}
			return left.p_item->comparer(right.p_item);
		}
	}pred;

	std::sort(m_grids.begin(), m_grids.end(), pred);
}

bool VecBag::getGridByIndex( uint32 index, BagGrid &grid ) const
{
	IF_RET(index>=m_grids.size(), false);
	grid = m_grids[index];
	return true;
}

bool VecBag::getGridByID(uint32 item_id, BagGrid &grid, uint32 &index) const
{
	index = 0;
	CONST_FOR_IT(VecGrid, m_grids)
	{
		if (NULL != it->p_item
			&& it->p_item->itemId()==item_id
			)
		{
			grid = *it;
			return true;
		}
		++index;
	}
	return false;
}

bool VecBag::find( const ItemIdMapNum &itemId_m_num ) const
{
	ItemIdMapNum mapDecItem = itemId_m_num;
	CONST_FOR_IT(VecGrid, m_grids)
	{
		if (NULL != it->p_item)
		{
			decMapItemIdNum(mapDecItem, it->p_item->itemId(), it->num);
		}
	}
	IF_RET(!mapDecItem.empty(), false);
	return true;
}

void VecBag::decMapItemIdNum( ItemIdMapNum &itemId_m_num, uint32 itemID, uint32 num ) const
{
	IF_RET_VOID(0 == itemID);
	ItemIdMapNum::iterator it=itemId_m_num.find(itemID);
	IF_RET_VOID(it==itemId_m_num.end());
	if(it->second <= num)
	{
		itemId_m_num.erase(it);
	}
	else
	{
		it->second-=num;
	}
}

bool VecBag::enableAttach( ItemBase *p_item) const
{
	IF_RET(0 == emptyGridNum(), false);
	return true;
}

bool VecBag::attach( ItemBase *p_item)
{
	IF_RET(!enableAttach(p_item), false);
	//加到空位置
	FOR_IT(VecGrid, m_grids)
	{
		if(NULL == it->p_item)
		{
			it->p_item = p_item;
			it->num = 1;
			return true;
		}
	}
	assert(0);
	return false;
}

bool VecBag::addItem( ItemBase *p_item, uint32 num)
{
	uint32 maxNum = p_item->maxPileNum();

	//优先叠加已有物品
	FOR_IT(VecGrid, m_grids)
	{
		if (NULL != it->p_item && p_item->uniqueId() == it->p_item->uniqueId())
		{
			if (num+it->num <= maxNum)
			{
				it->num = num+it->num;
				return true;
			} 
			else
			{
				num = num - (maxNum-it->num);
				it->num = maxNum;
				continue;
			}
		}
	}

	//加到空位置
	FOR_IT(VecGrid, m_grids)
	{
		if(NULL == it->p_item)
		{
			it->p_item = p_item;
			if (num <= maxNum)
			{
				it->num = num;
				return true;
			} 
			else
			{
				it->num = maxNum;
				num -= maxNum;
			}
			continue;
		}
	}
	assert(0);
	return false;
}

bool VecBag::enableAttach( uint32 index, ItemBase *p_item) const
{
	IF_RET(NULL == p_item, false);
	IF_RET(index >= m_grids.size(), false);
	IF_RET(NULL != m_grids[index].p_item, false);
	return true;
}

bool VecBag::attach( uint32 index, ItemBase *p_item)
{
	IF_RET(!enableAttach(index, p_item), false);
	m_grids[index].p_item = p_item;
	m_grids[index].num = 1;
	return true;
}

uint32 VecBag::emptyGridNum() const
{
	uint32 num=0;
	CONST_FOR_IT(VecGrid, m_grids)
	{
		if (NULL == it->p_item)
		{
			++num;
		}
	}
	return num;
}

bool VecBag::enableNewItem( const ItemIdMapNum &itemId_m_num ) const
{
	uint32 needGridNum=0;
	CONST_FOR_IT(ItemIdMapNum, itemId_m_num)
	{
		ItemBase *p_item = m_alloc.newItem(it->first);
		needGridNum+=checkAttachNeedGridNum(*p_item, it->second);
		m_alloc.delItem(p_item);
	}
	IF_RET(needGridNum > emptyGridNum(), false);
	return true;
}

uint32 VecBag::checkAttachNeedGridNum( ItemBase &item, uint32 num ) const
{
	uint32 gridNum = 0;
	uint32 maxPileNum = item.maxPileNum();
	//填满未满的，相同物品
	CONST_FOR_IT(VecGrid, m_grids)
	{
		if (NULL != it->p_item && it->p_item->uniqueId()==item.uniqueId())
		{
			uint32 remain = maxPileNum - it->num;
			if (num <= remain)
			{
				return gridNum;
			}
			num -=	remain;
		}
	}	
	gridNum = (num+maxPileNum-1)/maxPileNum;
	return gridNum;
}

bool VecBag::enableNewItem( uint32 item_id, uint32 num/*=1*/ ) const
{
	ItemBase *p_item = m_alloc.newItem(item_id);
	uint32 needGridNum=checkAttachNeedGridNum(*p_item, num);
	m_alloc.delItem(p_item);
	IF_RET(needGridNum > emptyGridNum(), false);
	return true;
}

bool VecBag::enableDelItem( const ItemIdMapNum &itemId_m_num ) const
{
	return find(itemId_m_num);
}

bool VecBag::enableDelItem( uint32 item_id, uint32 num/*=1*/ ) const
{
	ItemIdMapNum t;
	t.insert(make_pair(item_id,num));
	return find(t);
}

bool VecBag::exchangeItemPos( uint32 index1,uint32 index2 ) 
{
	if (index1>=m_grids.size() || index2>=m_grids.size())
	{
		return false;
	}
	swap(m_grids[index1], m_grids[index2]);
	return true;
}

bool VecBag::newItem( const ItemIdMapNum &itemId_m_num )
{
	IF_RET(!enableNewItem(itemId_m_num), false);
	CONST_FOR_IT(ItemIdMapNum, itemId_m_num)
	{
		if(!newItem(it->first, it->second))
		{
			assert(0);
			return false;
		}
	}
	return true;
}

bool VecBag::newItem( uint32 item_id, uint32 num/*=1*/ )
{
	if (!enableNewItem(item_id, num))
	{
		return false;
	}

	for (uint32 i=0; i<num; ++i)
	{
		ItemBase *p_item = m_alloc.newItem(item_id);
		if (p_item->isShare())
		{
			if(!addItem(p_item,num))
			{
				m_alloc.delItem(p_item);
				assert(0);
				return false;
			}
			return true;
		}
		else
		{
			if(!attach(p_item))
			{
				m_alloc.delItem(p_item);
				assert(0);
				return false;
			}
		}
	}
	return true;
}

bool VecBag::delItem( const ItemIdMapNum &itemId_m_num )
{
	IF_RET (!enableDelItem(itemId_m_num), false);
	CONST_FOR_IT(ItemIdMapNum, itemId_m_num)
	{
		uint32 index=0;
		{
			BagGrid grid; 
			if(!getGridByID(it->first, grid, index))
			{
				assert(0); return false;
			}
		}

		if(m_grids[index].num < it->second)
		{
			assert(0); return false;
		}
		m_grids[index].num -= it->second;
		if(0 == m_grids[index].num )
		{
			m_alloc.delItem(m_grids[index].p_item);
			m_grids[index].p_item = NULL;
		}
	}
	return true;
}

bool VecBag::delItem( uint32 item_id, uint32 num/*=1*/ )
{
	if (!enableDelItem(item_id, num))
	{
		return false;
	}

	uint32 index=0;
	{
		BagGrid grid; 
		if(!getGridByID(item_id, grid, index))
		{
			assert(0); return false;
		}
	}

	if(m_grids[index].num < num)
	{
		assert(0); return false;
	}
	m_grids[index].num -= num;
	if(0 == m_grids[index].num )
	{
		m_alloc.delItem(m_grids[index].p_item);
		m_grids[index].p_item = NULL;
	}
	return true;
}

bool VecBag::enableDetach( uint32 index ) const
{
	IF_RET(index >= m_grids.size(), false);
	IF_RET(NULL == m_grids[index].p_item, false);
	IF_RET(1 != m_grids[index].num, false);
	return true;
}

ItemBase * VecBag::detach( uint32 index )
{
	IF_RET(!enableDetach(index), NULL);
	ItemBase *p = m_grids[index].p_item;
	m_grids[index].num = 0;
	m_grids[index].p_item = NULL;
	return p;
}

void VecBag::addCapacity( uint32 addNum )
{
	uint32 len = m_grids.size()+addNum;
	m_grids.resize(len);
}

uint32 VecBag::capacity() const
{
	return m_grids.size();
}

void VecBag::read( Memory &out ) const
{
	uint32 len = m_grids.size();
	out << len;
	CONST_FOR_IT(VecGrid, m_grids)
	{
		if (NULL == it->p_item)
		{
			out << 0;
		}
		else
		{
			out << it->p_item->itemId();
			it->p_item->serialize(out);
			out << it->num;
		}
	}
}

void VecBag::write( const Memory &in )
{
	uint32 len = 0;
	in >> len;
	m_grids.resize(len);
	FOR_IT(VecGrid, m_grids)
	{
		uint32 item_id=0;
		in >> item_id;
		if (0 == item_id)
		{
		}
		else
		{
			ItemBase *p = m_alloc.newItem(item_id);
			if (NULL == p)
			{	
				in.setError();
				return;
			}
			p->unserialize(in);
			in >> it->num;
			it->p_item = p;
		}
	}
}

bool EquipmentBag::enableDetach( uint32 index ) const
{
	IF_RET(index>=m_items.size(), false);
	IF_RET(NULL == m_items[index], false);
	return true;
}

ItemBase * EquipmentBag::detach( uint32 index )
{
	IF_RET(!enableDetach(index), NULL);
	ItemBase *t = m_items[index];
	m_items[index] = NULL;
	return t;
}

bool EquipmentBag::enableAttach( uint32 index ) const
{
	IF_RET(index>=m_items.size(), false);
	IF_RET(NULL != m_items[index], false);
	return true;
}

bool EquipmentBag::attach( ItemBase *p_item, uint32 index )
{
	IF_RET(!enableAttach(index), false);
	m_items[index] = p_item;
	return true;
}

void EquipmentBag::read( Memory &out ) const
{
	uint32 len = m_items.size();
	out << len;
	CONST_FOR_IT(VecItem, m_items)
	{
		ItemBase *p = *it;
		if (NULL == p)
		{
			out << (uint32)0;
		}
		else
		{
			out << p->itemId();
			p->serialize(out);
		}
	}
}

void EquipmentBag::write( const Memory &in )
{
	uint32 len = 0;
	in >> len;
	m_items.resize(len);
	FOR_IT(VecItem, m_items)
	{
		uint32 item_id = 0;
		in >> item_id;
		if (0 == item_id)
		{
			*it = NULL;
		}
		else
		{
			ItemBase *p = m_alloc.newItem(item_id);
			p->unserialize(in);
			*it = p;
		}
	}
}

EquipmentBag::EquipmentBag( ItemAllocatee &alloc, uint32 capacity )
:m_alloc(alloc)
{
	m_items.resize(capacity);
	FOR_IT(VecItem, m_items)
	{
		*it = NULL;
	}
}
