/*
brief:��Ϸ������,
�¼����׹��ܵķ�����
1�������³�Ա������ FuntionName
2) cpp������� REG_CHEAT_FUN(FuntionName); ��̬ע�ắ����

�����ַ���";",",",".",":"," "�ֿ���
*/

#pragma once
#include "../utility/singleton.h"
#include "../utility/typedef.h"
#include "../utility/staticReg.h"
#include "../utility/cnt_typedef.h"

//typedef int GamePlayer;
class GamePlayer; //����࣬��ͬ�������ֲ�һ�����ֶ��İɡ�
class GameCheat : public SingletonBase<GameCheat>
{
public:
	// friend class SingletonBase<GameCheat>;

	typedef std::string (*ExecuteFun)(GamePlayer *pPlayer, const VecStr &vecStr);
public:
	//ִ�������ַ��������ؽ����Ϣ��	
	static std::string executeCheatStr(GamePlayer *pPlayer, const char *pChar);

public:
	static std::string doHandle1(GamePlayer *pPlayer, const VecStr &vecStr);
private:


};


//������̬ע����
REG_MAP_NAME_DECLARE(StrMapCheatFun, std::string, GameCheat::ExecuteFun)


