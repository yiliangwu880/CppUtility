

#include "mail.h"
#include <utility>

using namespace std;

bool MailServerBase::attachMail( uint32 playerID, MailBase *pMail )
{
	MapPlayerMail::iterator iter = m_mapPlayerMail.find(playerID);
	if (iter==m_mapPlayerMail.end())
	{
		iter = m_mapPlayerMail.insert(make_pair(playerID,MailBox())).first;
	}
	MailBox &mailBox = iter->second;
	mailBox.listMail.push_front(pMail);
	if (mailBox.listMail.size()>mailBox.maxNum)
	{
		mailBox.listMail.pop_back();
	}
	return true;

}

bool MailServerBase::dettachMail( uint32 playerID, MailBase *pMail )
{
	MapPlayerMail::iterator iter = m_mapPlayerMail.find(playerID);
	if (iter==m_mapPlayerMail.end())
	{
		iter = m_mapPlayerMail.insert(make_pair(playerID,MailBox())).first;
	}
	MailBox &mailBox = iter->second;

	for (MailBox::ListMail::iterator iterMail=mailBox.listMail.begin();
		iterMail!=mailBox.listMail.end(); ++iterMail)
	{
		if (pMail == *iterMail)
		{
			mailBox.listMail.erase(iterMail);
			return true;
		}
	}
	return false;
}

MailBox * MailServerBase::getPlayerMail( uint32 playerID )
{	
	MapPlayerMail::iterator iter = m_mapPlayerMail.find(playerID);
	if (iter==m_mapPlayerMail.end())
	{
		iter = m_mapPlayerMail.insert(make_pair(playerID,MailBox())).first;
	}
	return &(iter->second);
}
