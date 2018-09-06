/************************************************************************/
/*                                                                      
brief:
author:YiliangWu 
item base code
*/
/************************************************************************/



#include "item.h"

using namespace std;

ItemBase * ItemAllocatee::newItem( uint32 itemID )
{
	itemIdMapItem::iterator iter=m_shareItems.find(itemID);
	if (iter != m_shareItems.end())
	{
		return iter->second;
	}
	ItemBase *p_item = doNew(itemID);
	if (NULL == p_item)
	{
		return NULL;
	}
	if (p_item->isShare())
	{
		m_shareItems.insert(make_pair(itemID, p_item));
	}
	return p_item;
}

void ItemAllocatee::delItem( ItemBase *p_item )
{
	if (NULL == p_item)
	{
		return;
	}
	itemIdMapItem::iterator iter=m_shareItems.find(p_item->itemId());
	if (iter != m_shareItems.end())
	{
		return;
	}
	doDel(p_item);
}
