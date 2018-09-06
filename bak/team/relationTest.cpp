/*
test team serialize 
*/



#include "stdafx.h"
#include "relation.h"
#include "../utility/misc.h"

using namespace wyl;

namespace
{


	class MyMember:public RelationBase
	{
	public:
		MyMember()
		{
			m_id = 1;
		}
		virtual uint32 id() const{return m_id;};
		virtual void id(uint32 id){m_id=id;};
		virtual RelationBase *getPointObj(uint32 id) const;
		void clearRelation()
		{
			delAllPoint();
		}
		RelationBase *TeamPoint();  //return first TeamPoint

		uint32 m_id;
	};

	class MyTeam: public RelationBase
	{
	public:
		virtual uint32 id() const{return m_id;};
		virtual void id(uint32 id){m_id = id;};	
		virtual RelationBase *getPointObj(uint32 id) const
		{
			return NULL;
		};
		void clearRelation()
		{
			delAllPoint();
		}
		void dismiss()
		{
			delAllPoint();
		}
		void add(RelationBase *p)
		{
			addPoint(p);
		}
		void remove(uint32 index);
	private:
		uint32 m_id;
	};


	MyTeam team;
	MyMember member[10];
	char buf[1000];

	RelationBase * MyMember::getPointObj( uint32 id ) const
	{
		if (id == 1)
		{
			return &team;
		}
		return NULL;
	}

	void MyTeam::remove( uint32 index )
	{
		delPoint(&member[index]);
	}

	struct TeamMgr 
	{
		static MyTeam *find(uint32 id)
		{

			if(id == team.id())
			{
				return &team;
			}

			return NULL;
		}
	};

	RelationBase *MyMember::TeamPoint()  //return first TeamPoint
	{
		const RelationBase::SetPoint setID=getSetPoint();
		for (RelationBase::SetPoint::const_iterator iter=setID.begin();
			iter!=setID.end(); ++iter)
		{
			return TeamMgr::find((*iter)->id());
		}
		return NULL;
	}

	void clearTeamRelation()
	{
		team.clearRelation();
		for (int i=0; i<(int)ArrayLen(member); ++i)
		{
			member[i].clearRelation();
		}
	}
	struct SerialData 
	{
		uint32 num;
		char data[1];
	};
	void serialize(char *buf)
	{
		SerialData *p = (SerialData *)buf;
		for (int i=0; i<(int)ArrayLen(member); ++i)
		{
			p->num=member[i].serialize(p->data);
			p = (SerialData *)(p->data+p->num);
		}
	}
	void unSerialize(const char *buf)
	{
		SerialData *p = (SerialData *)buf;
		for (int i=0; i<(int)ArrayLen(member); ++i)
		{
			bool ret = member[i].unserialize(p->data);
			assert(ret);
			p = (SerialData *)(p->data+p->num);
		}
	}




	void printMember(RelationBase *p)
	{
		Info(p->id());
	}
	void printTeamMember()
	{
		const RelationBase::SetPoint setID=team.getSetPoint();
		for (RelationBase::SetPoint::const_iterator iter=setID.begin();
			iter!=setID.end(); ++iter)
		{
			Info((*iter)->id()); 
		}
	}
	void assertMemberAndTeamRelation()
	{

		for (int i=0; i<(int)ArrayLen(member); ++i)
		{
			const RelationBase::SetPoint setID=member[i].getSetPoint();
			for (RelationBase::SetPoint::const_iterator iter=setID.begin();
				iter!=setID.end(); ++iter)
			{
				assert((*iter)->isPoint(&member[i]));
			}
		}
	}
	void TeamTest1()
	{
		Info("TeamTest1 result:1,2,9");
		team.dismiss();
		team.add(&member[9]);
		team.add(&member[1]);
		team.add(&member[2]);
		printTeamMember();
		assert(member[9].TeamPoint()!=NULL);
		assert(member[1].TeamPoint()!=NULL);
		assert(member[2].TeamPoint()->id()==1);
		assert(member[3].TeamPoint()==NULL);
		assertMemberAndTeamRelation();

		serialize(buf);
		clearTeamRelation();
		unSerialize(buf);
		Info("----restore,result must equal----");

		printTeamMember();
		assert(member[9].TeamPoint()!=NULL);
		assert(member[1].TeamPoint()!=NULL);
		assert(member[2].TeamPoint()->id()==1);
		assert(member[3].TeamPoint()==NULL);
		assertMemberAndTeamRelation();
	}

	void TeamTest2()
	{
		Info("TeamTest2 result:2,9");
		team.dismiss();
		team.add(&member[9]);
		team.add(&member[1]);
		team.add(&member[2]);
		team.remove(1);

		printTeamMember();
		assert(member[1].TeamPoint()==NULL);
		assert(member[2].TeamPoint()!=NULL);
		assertMemberAndTeamRelation();

		serialize(buf);
		clearTeamRelation();
		unSerialize(buf);
		Info("----restore,result must equal----");

		printTeamMember();
		assert(member[1].TeamPoint()==NULL);
		assert(member[2].TeamPoint()!=NULL);
		assertMemberAndTeamRelation();
	}

	void TeamTest3()
	{
		Info("TeamTest3 result:");
		team.dismiss();
		team.add(&member[9]);
		team.add(&member[1]);
		team.add(&member[2]);
		team.remove(9);
		team.remove(1);
		team.remove(2);
		team.remove(2);

		printTeamMember();
		for (int i=0; i<(int)ArrayLen(member); ++i)
		{
			assert(member[i].TeamPoint()==NULL);
		}
		assertMemberAndTeamRelation();


		serialize(buf);
		clearTeamRelation();
		unSerialize(buf);
		Info("----restore,result must equal----");

		printTeamMember();
		for (int i=0; i<(int)ArrayLen(member); ++i)
		{
			assert(member[i].TeamPoint()==NULL);
		}
		assertMemberAndTeamRelation();
	}
	void appLyTeamTest();


	//test ApplyTeamBase ApplyMemberBase
	//////////////////////////////////////////////////////////////////////////
	class MyApplyMember:public ApplyMemberBase
	{
	public:
		MyApplyMember()
		{
			m_id = 1;
		}
		virtual uint32 id() const{return m_id;};
		virtual void id(uint32 id){m_id=id;};
		virtual ApplyTeamBase *doFindTeam(uint32 id);
		uint32 m_id;

		void clearTeamRelation()
		{
			cancelAllApply();
			delAllPoint();
		}

		RelationBase *getPointObj(uint32 id) const;
	};

	class MyApplyTeam: public ApplyTeamBase
	{
	public:
		virtual uint32 id() const {return m_id;};
		virtual void id(uint32 id){m_id = id;};
		void clearTeamRelation()
		{
			dimiss();
		}
		virtual RelationBase *getPointObj(uint32 id) const;

	private:
		uint32 m_id;
	};



	MyApplyTeam appTeam[10];
	MyApplyMember appMember[10];


	RelationBase * MyApplyTeam::getPointObj( uint32 id ) const
	{
		for (uint32 i=0; i<ArrayLen(appMember); ++i)
		{
			if (id == appMember[i].id())
			{
				return &appMember[i];
			}
		}
		return NULL;
	}


	struct ApplyTeamMgr
	{
		static MyApplyTeam *find(uint32 id)
		{
			for (uint32 i=0; i<ArrayLen(appTeam); ++i)
			{
				if (id == appTeam[i].id())
				{
					return &appTeam[i];
				}
			}
			return NULL;
		}
	};

	RelationBase * MyApplyMember::getPointObj( uint32 id )const
	{
		return ApplyTeamMgr::find(id);
	}

	ApplyTeamBase * MyApplyMember::doFindTeam( uint32 id )
	{
		return ApplyTeamMgr::find(id);
	}

	void clearApplyTeamRelation()
	{
		for (uint32 i=0; i<ArrayLen(appTeam); ++i)
		{
			appTeam[i].clearTeamRelation();
		}	
		for (uint32 i=0; i<ArrayLen(appMember); ++i)
		{
			appMember[i].clearTeamRelation();
		}
	}
	void serializeApply(char *buf)
	{
		SerialData *p = (SerialData *)buf;
		for (int i=1; i<(int)ArrayLen(appMember); ++i)
		{
			p->num=appMember[i].serialize(p->data);
			p = (SerialData *)(p->data+p->num);
		}
	}
	void unSerializeApply(const char *buf)
	{
		SerialData *p = (SerialData *)buf;
		for (int i=1; i<(int)ArrayLen(appMember); ++i)
		{
			bool ret = appMember[i].unserialize(p->data);
			assert(ret);
			p = (SerialData *)(p->data+p->num);
		}
	}


	void assertApplyMember(MyApplyTeam *pTeam, uint32 *pId)
	{
		if (0  == pId[0])
		{
			assert(pTeam->getSetApplyMember().empty());
			return;
		}
		uint32 num = 0;
		for (uint32 i=0; i<10; ++i)
		{
			if (0 == pId[i])
			{
				return;
			}
			MyApplyMember *pMember = dynamic_cast<MyApplyMember *>(pTeam->getPointObj(pId[i]));
			assert(NULL != pMember);
			assert(pTeam->isApplyMember(pMember));
			num++;
		}
		assert(num == pTeam->getSetApplyMember().size());

	}
	void assertApplyTeam(MyApplyMember *pMember, uint32 *pId)
	{
		if (0  == pId[0])
		{
			assert(pMember->getSetApplyTeam().empty());
			return;
		}
		uint32 num = 0;
		for (uint32 i=0; i<10; ++i)
		{
			if (0 == pId[i])
			{
				return;
			}			
			MyApplyTeam *pTeam = dynamic_cast<MyApplyTeam *>(pMember->getPointObj(pId[i]));
			assert(NULL != pTeam);

			MyApplyMember::SetApplyTeam::const_iterator iter = pMember->getSetApplyTeam().find(pTeam);
			assert(iter!=pMember->getSetApplyTeam().end());
			num++;
		}
		assert(num == pMember->getSetApplyTeam().size());
	}
	void printApplyMember(MyApplyTeam *pTeam)
	{
		MyApplyTeam::SetApplyMember::const_iterator iter = pTeam->getSetApplyMember().begin();
		for(; iter!=pTeam->getSetApplyMember().end(); ++iter)
		{
			Info((*iter)->id());
		}
	}

	void assertApplyMemberAndTeamRelation(MyApplyTeam *pTeam, MyApplyMember *pMember, int memberSize)
	{
		for (int i=0; i<memberSize; ++i)
		{
			for (ApplyMemberBase::SetApplyTeam::const_iterator iter=appMember[i].getSetApplyTeam().begin();
				iter!=appMember[i].getSetApplyTeam().end(); ++iter)
			{
				MyApplyTeam *pT = dynamic_cast<MyApplyTeam *> (appMember[i].getPointObj((*iter)->id()));
				assert(pT);
				pT->isApplyMember(&appMember[i]);
			}
		}
	}


	void clearApplyRelastion()
	{
		for (int i=0; i<(int)ArrayLen(appTeam); ++i)
		{
			typedef std::vector<uint32> VecPMember;
			VecPMember vecpMember;

			for (MyApplyTeam::SetApplyMember::const_iterator iter = appTeam[i].getSetApplyMember().begin();
				iter!=appTeam[i].getSetApplyMember().end(); ++iter)
			{
				vecpMember.push_back((*iter)->id());
			}


			for (VecPMember::iterator iter=vecpMember.begin();
				iter!=vecpMember.end(); ++iter)
			{

				ApplyMemberBase *pT = dynamic_cast<ApplyMemberBase *> (appTeam[i].getPointObj(*iter));
				appTeam[i].refuse(pT);
			}
		}
	}

	void applyTeamTest1()
	{

		appTeam[9].apply(&appMember[9]);

		{uint32 id[10]={9};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		serializeApply(buf);
		clearApplyTeamRelation();
		unSerializeApply(buf);

		{uint32 id[10]={9};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		clearApplyRelastion();
	}
	void applyTeamTest2()
	{

		appTeam[9].apply(&appMember[9]);
		appTeam[9].apply(&appMember[1]);

		{uint32 id[10]={9,1};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		serializeApply(buf);
		clearApplyTeamRelation();
		unSerializeApply(buf);

		{uint32 id[10]={9,1};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		clearApplyRelastion();
	}
	void applyTeamTest3()
	{

		appTeam[9].apply(&appMember[9]);
		appTeam[9].apply(&appMember[1]);
		appTeam[9].apply(&appMember[2]);
		appTeam[9].refuse(&appMember[1]);

		{uint32 id[10]={9,2};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		serializeApply(buf);
		clearApplyTeamRelation();
		unSerializeApply(buf);


		{uint32 id[10]={9,2};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		clearApplyRelastion();
		{uint32 id[10]={0};assertApplyMember(&appTeam[9], id);}
	}
	void applyTeamTest4()
	{

		appTeam[9].apply(&appMember[9]);
		appTeam[9].apply(&appMember[1]);
		appTeam[9].apply(&appMember[2]);
		appTeam[9].refuse(&appMember[1]);

		appTeam[9].apply(&appMember[1]);

		appTeam[9].refuse(&appMember[9]);
		appTeam[9].refuse(&appMember[1]);
		appTeam[9].refuse(&appMember[2]);

		{uint32 id[10]={0};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		serializeApply(buf);
		clearApplyTeamRelation();
		unSerializeApply(buf);

		{uint32 id[10]={0};assertApplyMember(&appTeam[9], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		clearApplyRelastion();
	}

	void applyTeamTest5()
	{
		appTeam[9].apply(&appMember[9]);
		appTeam[9].apply(&appMember[1]);
		appTeam[9].apply(&appMember[2]);
		appTeam[1].apply(&appMember[9]);
		appTeam[1].apply(&appMember[1]);
		appTeam[1].apply(&appMember[2]);


		{uint32 id[10]={9,1,2};assertApplyMember(&appTeam[9], id);}
		{uint32 id[10]={9,1,2};assertApplyMember(&appTeam[1], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		serializeApply(buf);
		clearApplyTeamRelation();
		unSerializeApply(buf);


		{uint32 id[10]={9,1,2};assertApplyMember(&appTeam[9], id);}
		{uint32 id[10]={9,1,2};assertApplyMember(&appTeam[1], id);}
		assertApplyMemberAndTeamRelation(appTeam, appMember, ArrayLen(appTeam));

		clearApplyRelastion();

		{uint32 id[10]={0};assertApplyMember(&appTeam[9], id);}
		{uint32 id[10]={0};assertApplyMember(&appTeam[1], id);}
	}
	//test remove obj
	void applyTeamTest6()
	{
		{
			clearApplyRelastion();
			appTeam[9].apply(&appMember[9]);
			appTeam[9].apply(&appMember[1]);
			appTeam[9].apply(&appMember[2]);
			appTeam[1].apply(&appMember[9]);
			appTeam[1].apply(&appMember[1]);
			appTeam[1].apply(&appMember[2]);

			appTeam[9].applove(&appMember[9]);
			appTeam[1].applove(&appMember[9]);
			{uint32 id[10]={1,9};assertApplyTeam(&appMember[1], id);}
			{uint32 id[10]={1,9};assertApplyTeam(&appMember[2], id);}
			{uint32 id[10]={1,2};assertApplyMember(&appTeam[9], id);}
			{uint32 id[10]={1,2};assertApplyMember(&appTeam[1], id);}


			appTeam[9].refuse(&appMember[2]);
			appTeam[1].refuse(&appMember[2]);
			{uint32 id[10]={1};assertApplyMember(&appTeam[9], id);}
			{uint32 id[10]={1};assertApplyMember(&appTeam[1], id);}
		}
	}
	//test remove obj
	void applyTeamTest7()
	{

	}


	void appLyTeamTest()
	{
		for (int i=0; i<(int)ArrayLen(appTeam); ++i)
		{
			appTeam[i].id(i);
		}
		appTeam[0].id(100);
		for (int i=0; i<(int)ArrayLen(appMember); ++i)
		{
			appMember[i].id(i);
		}
		appMember[0].id(100);
		applyTeamTest1();
		applyTeamTest2();
		applyTeamTest3();
		applyTeamTest4();
		applyTeamTest5();
		applyTeamTest6();
		applyTeamTest7();
		Info("end");
	}

} //namespace 

//²âÊÔÈë¿Ú
void TeamSerializeTest()
{
	team.id(1);
	for (int i=0; i<(int)ArrayLen(member); ++i)
	{
		member[i].id(i);
	}

	TeamTest1();
	TeamTest2();
	TeamTest3();
	Info("end");
	Info("////////////////////////appLyTeamTest start//////////////////////////////");
	appLyTeamTest();
}