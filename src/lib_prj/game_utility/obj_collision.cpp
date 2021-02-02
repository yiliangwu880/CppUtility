/*
brief:动态对象碰撞处理

*/
#include <assert.h>
#include "misc.h"
#include "obj_collision.h"
#define CONST_FOR_IT(type, ctn)\
    for(type::const_iterator it=(ctn).begin(); it!=(ctn).end(); ++it)
using namespace std;

void GameColObj::regHandle( const ColKey2Handle &handles, ObjTypeID new_class_id, ObjTypeID base_id)
{
	//bulid Key(old class, new class)
	ColKey2Handle ex_handles;
	CONST_FOR_IT(ColKey2Handle, handles)
	{
		ColKey key1(it->first.first, it->first.second);
		ColKey key2(it->first.second, it->first.first);
		ex_handles.insert(make_pair(key1, it->second));
		ex_handles.insert(make_pair(key2, it->second));
	}

	//get all reg class id
	set<ObjTypeID> setId;
	RegColHandle &reg = RegColHandle::obj();
	CONST_FOR_IT(RegColHandle, reg)
	{
		const ColKey& key = it->first;
		setId.insert(key.first);
		setId.insert(key.second);
	}

	//bulid reg handle list
	CONST_FOR_IT(set<ObjTypeID>, setId)
	{
		ColKey key1(new_class_id, *it);
		ColKey key2(*it, new_class_id);
		if (ex_handles.find(key1)==ex_handles.end())
		{//no new reg handle, derive the base calss handle;
			ColKey key(base_id, *it);
			RegColHandle::const_iterator it_1 = reg.find(key);
			if (it_1 == reg.end())
			{
				assert(false);
				return;
			}
			ex_handles.insert(make_pair(key1, it_1->second));
			ex_handles.insert(make_pair(key2, it_1->second));
		}
	}

	//reg handle
	CONST_FOR_IT(ColKey2Handle, ex_handles)
	{
		reg.insert(make_pair(it->first, it->second));
	}
}

void MyObj::setHanlde( ColKey2Handle &handles )
{

}
GAME_COL_OBJ_REG(MyObj, GameColObj)