

#include "simple_bag.h"
#include "../../utility/easyTest.h"
#include "../../utility/memoryStream.h"
#include "../../utility/misc.h"

using namespace std;
using namespace wyl;

MapBag::MapBag():m_grid_num(0), m_max_grid_num(1)
{

}

bool MapBag::enableNewItem( uint32 item_id, uint32 num ) const
{
	if(addItemNeedGridNum(item_id, num) > getFreeGridSize())
	{
		return false;
	}
	return true;
}

bool MapBag::enableNewItem( const IdMapNum &itemId_m_num ) const
{
	uint32 need_num=0;
	CONST_FOR_IT(IdMapNum, itemId_m_num)
	{
		need_num+=addItemNeedGridNum(it->first, it->second);
	}
	if(need_num > getFreeGridSize())
	{
		return false;
	}	
	return true;
}

void MapBag::setGridMaxSize( uint32 size )
{
	if (size > m_max_grid_num)
	{
		m_max_grid_num = size;
	}
}

uint32 MapBag::addItemNeedGridNum( uint32 item_id, uint32 num ) const
{
	ItemBase *p_item = alloc().newItem(item_id);
	uint32 ret = 0;
	const_iterator it = m_id_m_grid.find(p_item->uniqueId());
	if (it == m_id_m_grid.end())
	{
		ret = itemNumToGridNum(*p_item, num);
		alloc().delItem(p_item);
		return ret;
	}
	uint32 oldNum = itemNumToGridNum(*p_item, it->second.num);
	uint32 newNum = itemNumToGridNum(*p_item, it->second.num+num);
	ret = newNum-oldNum;
	
	alloc().delItem(p_item);
	return ret;
}

uint32 MapBag::itemNumToGridNum( const ItemBase &item, uint32 num ) const
{
	uint32 max_size = item.maxPileNum();
	return (num+max_size-1)/max_size;
}

bool MapBag::newItem( uint32 item_id, uint32 num )
{
	if(!enableNewItem(item_id, num))
	{
		return false;
	}
	ItemBase *p_item = alloc().newItem(item_id);
	iterator it = m_id_m_grid.find(p_item->uniqueId());
	if (it == m_id_m_grid.end())
	{
		m_grid_num += itemNumToGridNum(*p_item, num);
		m_id_m_grid.insert(make_pair(p_item->uniqueId(), BagGrid(num, p_item)));
	}
	else
	{
		uint32 oldNum = itemNumToGridNum(*p_item, it->second.num);
		it->second.num+=num;
		uint32 newNum = itemNumToGridNum(*p_item, it->second.num);
		m_grid_num += (newNum-oldNum);
	}
	return true;
}

bool MapBag::newItem( const IdMapNum &itemId_m_num )
{
	CONST_FOR_IT(IdMapNum, itemId_m_num)
	{
		if(!newItem(it->first, it->second))
		{
			return false;
		}
	}
	return true;
}

BagGrid * MapBag::findGridByItemId( uint32 item_id ) 
{
	FOR_IT(IdMapGrid, m_id_m_grid)
	{
		if(it->second.p_item->itemId() == item_id)
		{
			return &(it->second);
		}
	}
	return NULL;
}

bool MapBag::enableDelItem( uint32 item_id, uint32 num ) const
{
	const BagGrid *p = NULL;
	CONST_FOR_IT(IdMapGrid, m_id_m_grid)
	{
		if(it->second.p_item->itemId() == item_id)
		{
			p = &(it->second);
			break;
		}
	}

	if (NULL == p )
	{
		return false;
	}
	if (num > p->num)
	{
		return false;
	}
	return true;
}

bool MapBag::delItem( uint32 item_id, uint32 num )
{
	BagGrid *p = findGridByItemId(item_id);
	if (NULL == p )
	{
		return false;
	}
	if (num > p->num)
	{
		return false;
	}

	uint32 oldNum = itemNumToGridNum(*p->p_item, p->num);
	p->num -= num;
	uint32 newNum = itemNumToGridNum(*p->p_item, p->num);
	m_grid_num -= (oldNum-newNum);

	if (0 == p->num)
	{
		m_id_m_grid.erase(p->p_item->uniqueId());
	}
	return true;
}

bool MapBag::enableDelItem( const IdMapNum &itemId_map_num ) const
{
	CONST_FOR_IT(IdMapNum, itemId_map_num)
	{
		if(!enableDelItem(it->first, it->second))
		{
			return false;
		}
	}
	return true;
}

bool MapBag::delItem( const IdMapNum &itemId_m_num )
{
	CONST_FOR_IT(IdMapNum, itemId_m_num)
	{
		if(!delItem(it->first, it->second))
		{
			return false;
		}
	}
	return true;
}

MapBag::iterator MapBag::find( uint32 unique_id )
{
	return m_id_m_grid.find(unique_id);
}

void MapBag::erase( iterator it )
{
	alloc().delItem(it->second.p_item);
	m_id_m_grid.erase(it);
}

MapBag::iterator MapBag::begin()
{
	return m_id_m_grid.begin();
}

MapBag::iterator MapBag::end()
{
	return m_id_m_grid.end();
}

void MapBag::read( Memory &out ) const
{
	out << m_grid_num;
	out << m_max_grid_num;
	uint32 len = m_id_m_grid.size();
	out << len;
	CONST_FOR_IT(IdMapGrid, m_id_m_grid)
	{
		out << it->second.num;
		out << it->second.p_item->itemId();
		it->second.p_item->serialize(out);
	}
}

void MapBag::write( const Memory &in )
{
	in >> m_grid_num;
	in >> m_max_grid_num;
	uint32 len = 0;
	in >> len;
	for (uint32 i=0; i<len; ++i)
	{
		uint32 num=0; 
		in >> num;
		uint32 item_id = 0;
		in >> item_id;
		ItemBase *p = alloc().newItem(item_id);
		if (NULL == p)
		{
			in.setError();
			return;
		}
		p->unserialize(in);
	}
}

bool MapBag::enableDettach( uint32 unique_id ) const
{
	IdMapGrid::const_iterator it = m_id_m_grid.find(unique_id);
	if (it == m_id_m_grid.end())
	{
		return false;
	}
	return true;
}

ItemBase * MapBag::detach( uint32 unique_id )
{
	iterator it = find(unique_id);
	if (it == end())
	{
		return NULL;
	}
	ItemBase *p = it->second.p_item;
	--(it->second.num);
	if (0 == it->second.num)
	{
		m_id_m_grid.erase(it);
	}
	return p;
}

bool MapBag::enableAttach( ItemBase *p_item ) const
{
	if(addItemNeedGridNum(p_item->itemId(), 1) > getFreeGridSize())
	{
		return false;
	}
	return true;
}

bool MapBag::attach( ItemBase *p_item )
{
	IF_RET(NULL == p_item, false);
	uint32 need_num = 0;
	iterator it = find(p_item->uniqueId());
	if (it == end())
	{
		need_num = 1;

		IF_RET(need_num > getFreeGridSize(), false);
		m_id_m_grid.insert(make_pair(p_item->uniqueId(), BagGrid(1, p_item)));
		return true;
	}
	else
	{
		IF_RET(p_item!=it->second.p_item, false);
		uint32 oldNum = itemNumToGridNum(*p_item,  it->second.num);
		uint32 newNum = itemNumToGridNum(*p_item,  it->second.num+1);
		need_num = newNum-oldNum;
		IF_RET(need_num > getFreeGridSize(), false);	
		it->second.num+=1;
		return true;
	}
}

