/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/CppUtility.git

brief:ִ���ַ������ ��Ϸ������,
�ص㣺
	1���¼����׹��ܵķ����ǳ����㣬���ҿ��Է���һ���������� ����ȥ�����������ļ�����������壩
		�����º����� FuntionName
		�º������� REG_CHEAT_FUN(FuntionName); ��̬ע�ắ����
	2������ͺ�����һ���� ���� "AddMoney 1000" ��ִ�� AddMoney����. (�ž���ͬ���嶫��������һ�������������)

�ַ���������; , . : �ո� �ֿ�������

ȱ�㣺ʵ��ԭ�������


sample:
	GameCheat::DoCmd(player, "AddMoney 1000");


	//��������"AddMoney 1000"
	std::string AddMoney(GamePlayer *pPlayer, const VecStr &vecStr)
	{
		for(string v: vecStr)
		{
			....
		}
		return "ok";
	}
	REG_PLAYER_DO_CMD(AddMoney);

*/

#pragma once
#include "../utility/singleton.h"
#include "../utility/typedef.h"
#include "../utility/staticReg.h"
#include "../utility/cnt_typedef.h"

//typedef int GamePlayer;
class GamePlayer; //����࣬��ͬ�������ֲ�һ�����ֶ��İɡ�
using DoPlayerCmdFun = std::string(*)(GamePlayer *pPlayer, const VecStr &vecStr);
using DoCmdFun = std::string(*)(const VecStr &vecStr);
class GameCheat 
{
public:
	//ִ�������ַ��������ؽ����Ϣ��	
	static std::string DoCmd(GamePlayer *pPlayer, const char *pChar);
	static std::string DoCmd(const char *pChar);


};


//������̬ע����
REG_MAP_NAME_DECLARE(StrMapCheatFun, std::string, DoPlayerCmdFun)
REG_MAP_NAME_DECLARE(StrMapCheatFun_DoCmdFun, std::string, DoCmdFun)

//cmd ����Ҫ""
#define REG_PLAYER_DO_CMD_EX(cmd, fun)\
	MAP_REG_DEFINE(StrMapCheatFun, cmd, fun) 

#define REG_DO_CMD_EX(cmd, fun)\
	MAP_REG_DEFINE(StrMapCheatFun_DoCmdFun, cmd, fun) 

#define REG_PLAYER_DO_CMD(fun)\
	MAP_REG_DEFINE(StrMapCheatFun, fun, fun) 

#define REG_DO_CMD(fun)\
	MAP_REG_DEFINE(StrMapCheatFun_DoCmdFun, fun, fun) 
