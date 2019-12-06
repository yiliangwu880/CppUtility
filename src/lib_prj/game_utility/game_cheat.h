/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/CppUtility.git

brief:执行字符串命令， 游戏作弊码,
特点：
	1）新加作弊功能的方法非常方便，而且可以放在一个代码区域： （免去到处翻代码文件添加声明定义）
		定义新函数， FuntionName
		新函数后面 REG_CHEAT_FUN(FuntionName); 静态注册函数。
	2）命令和函数名一样。 比如 "AddMoney 1000" 就执行 AddMoney函数. (杜绝相同意义东西命名不一样，代码难浏览)

字符串命令用; , . : 空格 分开参数。

缺点：实现原理难理解


sample:
	GameCheat::DoCmd(player, "AddMoney 1000");


	//处理命令"AddMoney 1000"
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
class GamePlayer; //玩家类，不同工程名字不一样，手动改吧。
using DoPlayerCmdFun = std::string(*)(GamePlayer *pPlayer, const VecStr &vecStr);
using DoCmdFun = std::string(*)(const VecStr &vecStr);
class GameCheat 
{
public:
	//执行作弊字符串，返回结果信息。	
	static std::string DoCmd(GamePlayer *pPlayer, const char *pChar);
	static std::string DoCmd(const char *pChar);


};


//申明静态注册类
REG_MAP_NAME_DECLARE(StrMapCheatFun, std::string, DoPlayerCmdFun)
REG_MAP_NAME_DECLARE(StrMapCheatFun_DoCmdFun, std::string, DoCmdFun)

//cmd 不需要""
#define REG_PLAYER_DO_CMD_EX(cmd, fun)\
	MAP_REG_DEFINE(StrMapCheatFun, cmd, fun) 

#define REG_DO_CMD_EX(cmd, fun)\
	MAP_REG_DEFINE(StrMapCheatFun_DoCmdFun, cmd, fun) 

#define REG_PLAYER_DO_CMD(fun)\
	MAP_REG_DEFINE(StrMapCheatFun, fun, fun) 

#define REG_DO_CMD(fun)\
	MAP_REG_DEFINE(StrMapCheatFun_DoCmdFun, fun, fun) 
