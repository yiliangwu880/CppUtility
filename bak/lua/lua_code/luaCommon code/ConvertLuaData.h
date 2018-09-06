#pragma once


#include "lua.hpp"
#include <string>
#include <assert.h>

typedef std::string string;

class CLuaTableError
{
public:
	CLuaTableError(string msg);
	const string &GetMsg(void);
private:
	string m_msg;
};

/*
@brief:read or write table info on lua stack
remark:
	it will throw CLuaTableError when read or write an error lua data.
*/
class CLuaTable
{
public:
	/*
	@brief:read or write stack
	@parameter: int index, identify the table position in lua stack
	@remark: index value must be negative
	*/
	CLuaTable(lua_State *pLuaState, int index);
	
	/*
	@brief:read or write stack
	@parameter:bool readFlag, value == true specify read from stack,else write form stack.
	@remark:
	*/
	void ExchangeNumber(double *pData, const char *key, bool readFlag);
	void ExchangeNumber(double *pData, int key, bool readFlag);
	void ExchangeBoolean(bool *pData, const char *key, bool readFlag);
	void ExchangeBoolean(bool *pData, int key, bool readFlag);

	void ExchangeString(string *pData, const char *key, bool readFlag, uint *pGetLength = NULL);
	void ExchangeString(string *pData, int key, bool readFlag, uint *pGetLength = NULL);

	uint GetArrayLen(void);
	/*
	@breif:create sub table for read or write.
	@parameter:bool readFlag, value == true specify read from stack,else write form stack.
	@remark:call this function will push a table in stack.
		for purpose make current stack recover original state,
		must call "void CloseSubTable(void);" when finish read or write handle of subtable.
		example:
		{
		CLuaTable subTable = father.CreateSubTable("buildings", readFlag);
			subTable.ExchangeNumber(...);
			....
		father.CloseSubTable(readFlag);
		}
	*/
	CLuaTable CreateSubTable(const char *key, bool readFlag);
	CLuaTable CreateSubTable(int key, bool readFlag);

	/*
	breif:reference "CLuaTable CreateSubTable(const char *key, bool readFlag);"
	*/
	void CloseSubTable(bool readFlag);

	void CLuaTable::StartGetNextTable(void);
	void CLuaTable::EndGetNextTable(void);
	/*
	@breif:
	@parameter:
	@remark:为测试成功
	example:
		{
			int getKey;
			bool endFlag;
			father.StartGetNextTable();
			CLuaTable subTable = father.CreateNextTable(&getKey, readFlag, &endFlag);
			if(!endFlag)
			{
				subTable.ExchangeNumber(...);
					....
				father.CloseSubTable(readFlag);
			}
			else
			{

			}
			father.EndGetNextTable();
		}
	*/
	CLuaTable CLuaTable::CreateNextTable(int *pKey, bool readFlag, bool *pEndFlag);

	lua_State * GetLuaState(void);

	int GetLuaStateIndex(void);
private:
	double GetNumber(const char *key);
	double GetNumber(int key);
	bool GetBoolean(const char *key);
	bool GetBoolean(int key);

	/*
	breif:获取string,
	@parameter: uint *pLength ,[out] string的长度
	@remark:
	返回的长度有可能 > strlen(返回值)。
	*/
	const char *GetString(const char *key, uint *pLength);

	//brief:Equivalent to "const char *GetString(const char *key, uint *pLength);"
	const char *GetString(int key, uint *pLength);

	void SetNumber(const char *key, double v);
	void SetNumber(int key, double v);
	void SetBoolean(const char *key, bool b);
	void SetBoolean(int key, bool b);

	/*
	breif:write string
	*/
	void SetString(const char *key, const char *pString);

	/*
	breif:Equivalent to "void SetString(const char *key, const char *pString);"
	*/
	void SetString(int key, const char *pString);

	/*
	breif:push subTable in lua_State stack top.
	remark: after complete the subTable ,It is better to call member function "PopSubTable" .
			没完成GetAndPushSubTable前，不可以连续调用多次
	*/
	CLuaTable GetAndPushSubTable(const char *key);

	/*
	breif:push subTable in lua_State stack top.
	*/
	CLuaTable GetAndPushSubTable(int key);

	/*
	breif:pop subTable
	*/
	void PopSubTable(void);

	/*
	@breif:handle like funtion "lua_newtable"
		push it in lua_State stack top.
	@remark: after complete the set subTable ,call member function "SetAndPopSubTable" to set sub Table to father table.
			没完成SetAndPopSubTable前，不可以连续调用多次
	*/
	CLuaTable NewAndPushSubTable(void);

	/*
	breif:handle like funtion "lua_settable"
	*/
	void SetAndPopSubTable(const char *key);

	/*
	breif:Equivalent to funtion "void SetAndPopSubTable(const char *key);"
	*/
	void SetAndPopSubTable(int key);


private:
	lua_State *m_luaState;
	int m_pushCnt;
	int m_index;
	string m_subTable;
	int m_subTableIndex;
};
