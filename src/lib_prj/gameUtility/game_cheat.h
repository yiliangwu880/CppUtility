/*
brief:游戏作弊码,
新加作弊功能的方法：
1）定义新成员函数， FuntionName
2) cpp里面添加 REG_CHEAT_FUN(FuntionName); 静态注册函数。

作弊字符用";",",",".",":"," "分开。
*/

#pragma once
#include "../utility/singleton.h"
#include "../utility/typedef.h"
#include "../utility/staticReg.h"
#include "../utility/cnt_typedef.h"

//typedef int GamePlayer;
class GamePlayer; //玩家类，不同工程名字不一样，手动改吧。
class GameCheat : public SingletonBase<GameCheat>
{
public:
	// friend class SingletonBase<GameCheat>;

	typedef std::string (*ExecuteFun)(GamePlayer *pPlayer, const VecStr &vecStr);
public:
	//执行作弊字符串，返回结果信息。	
	static std::string executeCheatStr(GamePlayer *pPlayer, const char *pChar);

public:
	static std::string doHandle1(GamePlayer *pPlayer, const VecStr &vecStr);
private:


};


//申明静态注册类
REG_MAP_NAME_DECLARE(StrMapCheatFun, std::string, GameCheat::ExecuteFun)


