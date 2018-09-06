/************************************************************************/
/*                  
参考用，没复用价值
brief:
author:YiliangWu 
程序开始，先创建队伍读库，再创建队友读库（构建关系）。
*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <set>

//基本关系类，直接派生可以作为队伍，成员类。
//attention: 如果是队伍对象，不需要存库（队员已经保存关系）。
class RelationBase 
{
public:
	typedef std::set<RelationBase *> SetPoint; 
public:
	virtual ~RelationBase(){}
	void addPoint(RelationBase *pOther);
	void delPoint(RelationBase *pOther);
	void delAllPoint();
	bool isPoint(RelationBase *pOther) const;
	const SetPoint &getSetPoint() const;
	uint32 serialize(char *out) const;
	bool unserialize( const char *in);
	virtual uint32 id() const=0;
private:
	bool attach(RelationBase *);
	bool dettach(RelationBase *);

protected:
	virtual RelationBase *getPointObj(uint32 id) const=0; //get Point Class obj, usual dif from self class.

private:
	SetPoint m_setPoint;			
};


class ApplyMemberBase;
//申请功能组队
class ApplyTeamBase: public RelationBase
{
public:
	friend class ApplyMemberBase;

	typedef std::set<ApplyMemberBase *> SetApplyMember; 
public:
	bool apply(ApplyMemberBase *pMember);
	bool applove(ApplyMemberBase *pMember);
	void refuse(ApplyMemberBase *pMember);
	bool isApplyMember(ApplyMemberBase *pMember);
	void dimiss();
	const SetApplyMember &getSetApplyMember() const;
private:
	void beCancelApply(ApplyMemberBase *pMember);
	void attachApply(ApplyMemberBase *pMember);
private:
	SetApplyMember m_setApplyMember;

};


class ApplyMemberBase : public RelationBase
{
public:
	friend class ApplyTeamBase;

	typedef std::set<ApplyTeamBase *> SetApplyTeam; 
public:
	bool isApply(ApplyTeamBase *pTeam);
	bool cancelApply(ApplyTeamBase *pTeam);
	void cancelAllApply();
	const SetApplyTeam &getSetApplyTeam() const;
	uint32 serialize(char *out) const;
	bool unserialize( const char *in);

private:
	void beAppove(ApplyTeamBase *pTeam);
	void addApplyTeam(ApplyTeamBase *pTeam);
	void beRefuse(ApplyTeamBase *pTeam);	
	virtual void onBeApplove(ApplyTeamBase *pTeam){};
	virtual void onApplyTeam(ApplyTeamBase *pTeam){};
	virtual void onBeRefuse(ApplyTeamBase *pTeam){};	

protected:
	SetApplyTeam m_setApplyTeam;
};