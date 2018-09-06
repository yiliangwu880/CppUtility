/************************************************************************/
/*                 
���ο���û���ü�ֵ
brief:
author:YiliangWu 

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <list>
#include <vector>
#include <unordered_map>


class MailBase
{
public:
	virtual ~MailBase()=0;
	uint32 type;
	
};
struct MailBox 
{
	//MailBox():isDirty(false){}
	typedef std::list<MailBase *> ListMail;
	ListMail listMail;
	uint32 maxNum;
	bool isDirty;	//true��ʾ�޸Ĺ�����Ҫ�浵
};

class MailServerBase
{
	typedef std::unordered_map<uint32, MailBox> MapPlayerMail;		//key == player id
public:
	
	bool attachMail(uint32 playerID, MailBase *pMail);
	bool dettachMail(uint32 playerID, MailBase *pMail);
	MailBox *getPlayerMail(uint32 playerID);

protected:
	MapPlayerMail m_mapPlayerMail;
};

