/************************************************************************/
/*                                                                      
brief:
author:YiliangWu 
bag base code
*/
/************************************************************************/

#pragma once

#include "../../utility/typedef.h"
#include "../../utility/memoryStream.h"
#include <vector>
#include <map>
#include "item.h"

class ItemBase;
class VecBag;

//������֧�ֶ�̬��Ʒ����¼λ��,����������
class VecBag
{
public:
	typedef std::map<uint32, uint32> ItemIdMapNum; //key = itemID, mapped_value = num
	typedef std::vector<BagGrid> VecGrid;

public:
	VecBag(ItemAllocatee &alloc);
	VecBag(ItemAllocatee &alloc, uint32 capacity);
	~VecBag();
	bool getGridByIndex( uint32 index, BagGrid &grid ) const;
	bool getGridByID(uint32 itemID, BagGrid &grid, uint32 &index) const;

	//������ɾ��������Ʒ����
	//////////////////////////////////////////////////////////////////////////
	bool enableNewItem(const ItemIdMapNum &itemId_m_num) const;
	bool enableNewItem(uint32 item_id, uint32 num=1) const;
	bool enableDelItem(const ItemIdMapNum &itemId_m_num) const;
	bool enableDelItem(uint32 item_id, uint32 num=1) const;

	bool newItem(const ItemIdMapNum &itemId_m_num);
	bool newItem(uint32 item_id, uint32 num=1);
	bool delItem(const ItemIdMapNum &itemId_m_num);
	bool delItem(uint32 item_id, uint32 num=1);
	//////////////////////////////////////////////////////////////////////////

	bool exchangeItemPos(uint32 index1,uint32 index2);

	//����Ʒָ�������ע�������Ķ��󣬲�ʹ�õ�ʱ��Ҫ��alloc().delItem����
	//////////////////////////////////////////////////////////////////////////
	bool enableDetach(uint32 index) const;
	ItemBase *detach(uint32 index);
	bool enableAttach(ItemBase *p_item) const;
	bool attach(ItemBase *pItem);
	bool enableAttach(uint32 index, ItemBase *p_item) const;
	bool attach(uint32 index, ItemBase *pItem);
	//////////////////////////////////////////////////////////////////////////

	void read(Memory &out) const;
	void write(const Memory &in);
	void sort(); //����
	uint32 capacity() const;
	void addCapacity(uint32 addNum);
	uint32 emptyGridNum() const;
private:
	bool addItem( ItemBase *p_item, uint32 num);
	bool find(const ItemIdMapNum &itemId_m_num) const;
	uint32 checkAttachNeedGridNum( ItemBase &item, uint32 num ) const;
	void decMapItemIdNum( ItemIdMapNum &itemId_m_num, uint32 itemID, uint32 num ) const;
private:
	ItemAllocatee &m_alloc;
	VecGrid m_grids;
};


//������װ������
class EquipmentBag 
{
public:
	EquipmentBag(ItemAllocatee &alloc, uint32 capacity);
	bool enableDetach(uint32 index) const;
	ItemBase *detach(uint32 index);
	bool enableAttach(uint32 index) const;
	bool attach(ItemBase *pItem, uint32 index);

	void read(Memory &out) const;
	void write(const Memory &in);
private:

protected:
	typedef std::vector<ItemBase *> VecItem;

	ItemAllocatee &m_alloc;
	VecItem m_items;
};