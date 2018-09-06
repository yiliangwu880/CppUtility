/*
brief: 组队相关
one member map one team, one team map more member.
attention:
程序结束时，存库，然后直接调用删除队伍和队员对象。
程序开始，先创建队伍读库，再创建队友读库（构建关系）。

未完成
*/

#pragma once

#include "../utility/typedef.h"
#include <map>
#include <set>

class TeamMemberBase;
class TeamBase  
{
public:
 typedef std::map<uint32, TeamMemberBase*> MapType;
 typedef MapType::iterator MemberIter;

 friend class TeamMemberBase;

public:
	TeamBase();
	virtual ~TeamBase();
	bool add(TeamMemberBase *pMember);
	void remove(uint32 id);
	void dismiss();
	bool isMember(TeamMemberBase *pMember);
	virtual uint32 id() const =0;
	virtual void id(uint32 id)=0;
	MemberIter beginMember();
	MemberIter endMember();

private:
	bool attachMember(TeamMemberBase *pMember); //direct init data member

protected:
	MapType m_map;
};

class TeamMemberBase
{
	friend class TeamBase;
public:
	TeamMemberBase();
	virtual ~TeamMemberBase();
	virtual uint32 id() const = 0;
	virtual void id(uint32 id)=0;
	TeamBase *TeamPoint(){return m_pTeam;};
	uint32 serialize(char *out) const;
	bool unserialize( const char *in);

protected:
	virtual TeamBase *doFindTeam(uint32 id)=0; //find in all exist team

private:
	void joinTeam(TeamBase * pTeam);
	void leaveTeam();
	virtual void onJoinTeam(TeamBase * pTeam){};
	virtual void onLeaveTeam(){};
	bool attachTeam(TeamBase *pTeam); //direct init add team
	
protected:
	TeamBase *m_pTeam;
};

//申请功能组队
class ApplyMemberBase;
class ApplyTeamBase: public TeamBase
{
public:
	typedef std::set<ApplyMemberBase *> SetType;
	typedef SetType::iterator ApplyMemberIter;

	friend class ApplyMemberBase;

public:
	~ApplyTeamBase();
	bool apply(ApplyMemberBase *pMember);
	bool applove(ApplyMemberBase *pMember);
	void refuse(ApplyMemberBase *pMember);
	bool isApplyMember(ApplyMemberBase *pMember);
	ApplyMemberIter beginApplyMember();
	ApplyMemberIter endApplyMember();

private:
	//capsulation and hide fun of base class;
	bool add(TeamMemberBase *pMember);
	bool attachApplyMember(ApplyMemberBase *pMember);

protected:
	SetType m_mapApplyMembers;
};


class ApplyMemberBase : public TeamMemberBase
{
public:
	typedef std::set<ApplyTeamBase *> SetType;
	typedef SetType::iterator ApplyTeamIter;

	friend class ApplyTeamBase;

public:
	virtual ~ApplyMemberBase();
	bool isApply(ApplyTeamBase *pTeam);
	ApplyTeamIter beginApplyTeam();
	ApplyTeamIter endApplyTeam();
	uint32 serialize(char *out) const;
	bool unserialize( const char *in);

protected:
	virtual ApplyTeamBase *doFindTeam(uint32 id)=0;//find in all exist team

private:
	void beApplyTeam(ApplyTeamBase *pTeam);
	void beRefuse(ApplyTeamBase *pTeam);	
	virtual void onApplyTeam(ApplyTeamBase *pTeam){};
	virtual void onBeRefuse(ApplyTeamBase *pTeam){};	
	bool attachApplyTeam(ApplyTeamBase *pTeam);//direct add apply team

protected:
	SetType m_setApplyTeam;
};