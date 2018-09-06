
#include "ConvertLuaData.h"



CLuaTable::CLuaTable(lua_State *pLuaState, int index)
:m_luaState(pLuaState),
m_pushCnt(0),
m_index(index),
m_subTableIndex(-1)
{
	if(!lua_istable(m_luaState, m_index))
	{
		throw CLuaTableError("create lua stack position isn't table");
	}
}

CLuaTable CLuaTable::GetAndPushSubTable(const char *key)
{
	lua_pushstring(m_luaState, key); 
	lua_gettable(m_luaState, m_index-1); /* get background[key] */
	if(!lua_istable(m_luaState, -1))
	{
		string s(key);
		s+=" isn't table";
		throw CLuaTableError(s);
	}
	m_pushCnt++;
	m_index--;

	return CLuaTable(m_luaState, -1);
}  

CLuaTable CLuaTable::GetAndPushSubTable(int key)
{
	assert(0 == m_pushCnt);
	lua_rawgeti(m_luaState,m_index, key);
	if(!lua_istable(m_luaState, -1))
	{
		char buf[20];
		_ltoa_s(key, buf, 20, 10);
		string s(buf);
		s+=" isn't table";
		throw CLuaTableError(s);
	}
	m_pushCnt++;
	m_index--;

	return CLuaTable(m_luaState, -1);
}

void CLuaTable::PopSubTable(void)
{
	m_pushCnt--;
	m_index++;
	assert(m_pushCnt>=0);
	assert(m_index<0);
	lua_pop(m_luaState, 1);
}

CLuaTable CLuaTable::NewAndPushSubTable(void)
{
	assert(0 == m_pushCnt);
	lua_newtable(m_luaState);  
	m_pushCnt++;
	m_index--;
	return CLuaTable(m_luaState, -1);
}

void CLuaTable::SetAndPopSubTable(const char *key)
{
	lua_pushstring(m_luaState, key);
	lua_insert(m_luaState,-2);
	lua_settable(m_luaState,m_index-1);  
	m_pushCnt--;
	m_index++;
	assert(m_pushCnt>=0);
	assert(m_index<0);
}

void CLuaTable::SetAndPopSubTable(int key)
{
	lua_pushnumber(m_luaState, key);
	lua_insert(m_luaState,-2);
	lua_settable(m_luaState,m_index-1);  
	m_pushCnt--;
	m_index++;
	assert(m_pushCnt>=0);
	assert(m_index<0);
}

double CLuaTable::GetNumber(const char *key)
{
	double r;

	lua_getfield(m_luaState, m_index, key);
	if(!lua_isnumber(m_luaState, -1))	
	{
		string s(key);
		s+=" isn't number";
		throw CLuaTableError(s);
	}
	r = lua_tonumber(m_luaState,-1);
	lua_pop(m_luaState, 1);
	return r;
}

double CLuaTable::GetNumber(int key)
{
	double r;

	lua_rawgeti(m_luaState, m_index, key);
	if(!lua_isnumber(m_luaState, -1))
	{
		char buf[20];
		_ltoa_s(key, buf, 20, 10);
		string s(buf);
		s+=" isn't number";
		throw CLuaTableError(s);
	}
	r = lua_tonumber(m_luaState,-1);
	lua_pop(m_luaState, 1);
	return r;
}

bool CLuaTable::GetBoolean(const char *key)
{
	bool r;

	lua_getfield(m_luaState, m_index, key);
	if(!lua_isboolean(m_luaState, -1))	
	{
		string s(key);
		s+=" isn't bool";
		throw CLuaTableError(s);
	}
	int i = lua_toboolean(m_luaState,-1);
	if(i)
	{
		r = true;
	}
	else
	{
		r = false;
	}
	lua_pop(m_luaState, 1);
	return r;
}

bool CLuaTable::GetBoolean(int key)
{
	bool r;

	lua_rawgeti(m_luaState, m_index, key);
	if(!lua_isboolean(m_luaState, -1))	
	{
		char buf[20];
		_ltoa_s(key, buf, 20, 10);
		string s(buf);
		s+=" isn't bool";
		throw CLuaTableError(s);
	}
	int i = lua_toboolean(m_luaState,-1);
	if(i)
	{
		r = true;
	}
	else
	{
		r = false;
	}
	lua_pop(m_luaState, 1);
	return r;
}

const char *CLuaTable::GetString(const char *key, uint *pLength)
{
	const char * r;

	lua_getfield(m_luaState, m_index, key);
	if(!lua_isstring(m_luaState, -1))
	{
		string s(key);
		s+=" isn't string";
		throw CLuaTableError(s);
	}
	r = lua_tolstring(m_luaState,- 1, pLength);
	lua_pop(m_luaState, 1);
	return r;
}

const char *CLuaTable::GetString(int key, uint *pLength)
{
	const char * r;

	lua_rawgeti(m_luaState, m_index, key);
	if(!lua_isstring(m_luaState, -1))
	{
		char buf[20];
		_ltoa_s(key, buf, 20, 10);
		string s(buf);
		s+=" isn't string";
		throw CLuaTableError(s);
	}
	r = lua_tolstring(m_luaState, -1, pLength);
	lua_pop(m_luaState, 1);
	return r;
}

void CLuaTable::SetNumber(const char *key, double v)
{
	lua_pushstring(m_luaState,key);
	lua_pushnumber(m_luaState, v);
	lua_settable(m_luaState,m_index-2);  	
}

void CLuaTable::SetNumber(int key, double v)
{
	lua_pushnumber(m_luaState,v);
	lua_rawseti(m_luaState, m_index-1, key);
}

void CLuaTable::SetBoolean(const char *key, bool b)
{
	lua_pushstring(m_luaState,key);
	lua_pushboolean(m_luaState, b);
	lua_settable(m_luaState,m_index-2);  
}

void CLuaTable::SetBoolean(int key, bool b)
{
	lua_pushboolean(m_luaState,b);
	lua_rawseti(m_luaState, m_index-1, key);
}

void CLuaTable::SetString(const char *key, const char *pString)
{
	lua_pushstring(m_luaState,key);
	lua_pushstring(m_luaState, pString);
	lua_settable(m_luaState,m_index-2);  	
}

void CLuaTable::SetString(int key, const char *pString)
{
	lua_pushstring(m_luaState,pString);
	lua_rawseti(m_luaState, m_index-1, key);
}

CLuaTable CLuaTable::CreateSubTable(const char *key, bool readFlag)
{
	if( !m_subTable.empty() || -1 != m_subTableIndex)
	{
		throw CLuaTableError("call CreateSubTable error, CreateSubTable should be match with CloseSubTable,Pls refer the function description");
	}
	m_subTable = key;
	if (readFlag)
	{
		return GetAndPushSubTable(m_subTable.c_str());
	} 
	else
	{
		return NewAndPushSubTable();
	}
}

CLuaTable CLuaTable::CreateSubTable(int key, bool readFlag)
{
	if( !m_subTable.empty() || -1 != m_subTableIndex)
	{
		throw CLuaTableError("call CreateSubTable error, CreateSubTable should be match with CloseSubTable,Pls refer the function description");
	}
	if( -1 == key)
	{
		throw CLuaTableError("CreateSubTable parameter key can't be -1");
	}
	m_subTableIndex = key;
	if (readFlag)
	{
		return GetAndPushSubTable(key);
	} 
	else
	{
		return NewAndPushSubTable();
	}
}

lua_State * CLuaTable::GetLuaState(void)
{
	return m_luaState;
}	

int CLuaTable::GetLuaStateIndex(void)
{
	return m_index;
}	

void CLuaTable::StartGetNextTable(void)
{
	lua_pushnil(m_luaState); 
	m_pushCnt++;
	m_index--;
}	

void CLuaTable::EndGetNextTable(void)
{
	lua_pop(m_luaState,1); 
	m_pushCnt--;
	m_index++;
}	

CLuaTable CLuaTable::CreateNextTable(int *pKey, bool readFlag, bool *pEndFlag)
{
	if( !m_subTable.empty() || -1 != m_subTableIndex)
	{
		throw CLuaTableError("call CreateSubTable error, CreateSubTable should be match with CloseSubTable,Pls refer the function description");
	}

	if (readFlag)
	{
		double r;
		assert(1 == m_pushCnt);//是否还没调用 InitGetNextTable ？
		if(lua_next(m_luaState,m_index))
		{
			if(!lua_isnumber(m_luaState, -2))
			{
				throw CLuaTableError("CreateNextTable key isn't number");
			}
			r = lua_tonumber(m_luaState,-2);//key
			*pKey = static_cast<int>(r);
			m_subTableIndex = *pKey; 
			if(!lua_istable(m_luaState, -1))
			{
				throw CLuaTableError("next member isn't table");
			}
			m_pushCnt++;
			m_index--;
			*pEndFlag = false;
		}
		else
		{
			lua_pop(m_luaState,1); 
			*pEndFlag = true;
		}
		return CLuaTable(m_luaState, -1);
	} 
	else
	{
		throw CLuaTableError("CreateNextTable readFlag can't be false");
	}
}

void CLuaTable::CloseSubTable(bool readFlag)
{
	if(m_subTable.empty() && -1 == m_subTableIndex)
	{
		throw CLuaTableError("call CloseSubTable error, CreateSubTable should be match with CloseSubTable,Pls refer the function description");
	}
	if (readFlag)
	{
		PopSubTable();
	} 
	else
	{
		if(!m_subTable.empty())
		{
			SetAndPopSubTable(m_subTable.c_str());
		}
		else if (-1 != m_subTableIndex)
		{
			SetAndPopSubTable(m_subTableIndex);
		}
		else
		{
			throw CLuaTableError("call CloseSubTable error, CreateSubTable should be match with CloseSubTable,Pls refer the function description");
		}
	}
	m_subTableIndex = -1;
	m_subTable.clear();
}

uint CLuaTable::GetArrayLen(void)
{
	uint r = luaL_getn(m_luaState,m_index);
	//lua_pop(m_luaState,1);
	return r;
}

void CLuaTable::ExchangeNumber(double *pData, const char *key, bool readFlag)
{
	if (readFlag)
	{
		 *pData = GetNumber(key);
	} 
	else
	{
		SetNumber(key, *pData);
	}
}

void CLuaTable::ExchangeNumber(double *pData, int key, bool readFlag)
{
	if (readFlag)
	{
		*pData = GetNumber(key);
	} 
	else
	{
		SetNumber(key, *pData);
	}
}

void CLuaTable::ExchangeBoolean(bool *pData, const char *key, bool readFlag)
{
	if (readFlag)
	{
		*pData = GetBoolean(key);
	} 
	else
	{
		SetBoolean(key, *pData);
	}
}

void CLuaTable::ExchangeBoolean(bool *pData, int key, bool readFlag)
{
	if (readFlag)
	{
		*pData = GetBoolean(key);
	} 
	else
	{
		SetBoolean(key, *pData);
	}
}

void CLuaTable::ExchangeString(string *pData, const char *key, bool readFlag, uint *pGetLength)
{
	if (readFlag)
	{
		*pData = GetString(key, pGetLength);
	} 
	else
	{
		SetString(key, pData->c_str());
	}
}

void CLuaTable::ExchangeString(string *pData, int key, bool readFlag, uint *pGetLength)
{
	if (readFlag)
	{
		*pData = GetString(key, pGetLength);
	} 
	else
	{
		SetString(key, pData->c_str());
	}
}



CLuaTableError::CLuaTableError(string msg)
:m_msg(msg)
{

}

const string &CLuaTableError::GetMsg(void)
{
	return m_msg;
}