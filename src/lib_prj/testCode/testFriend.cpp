


#include "stdafx.h"


//#include "../game_utility/res.h"
#include "../game_utility/friend.h"
#include "../utility/misc.h"

using namespace std;
using namespace wyl;

namespace
{
	//relation
	//////////////////////////////////////////////////////////////////////////
	class Player: public OneWayRelationBase
	{
	public:
		Player(uint32 id):m_id(id){}
		virtual uint32 id() const{return m_id;};
		virtual OneWayRelationBase *getObjById(uint32 id) const;

		bool point(uint32 *p);
		bool bePoint(uint32 *p);
		void clear()
		{
			const SetPoint setPoint = getSetPoint();
			for(SetPoint::const_iterator iter = setPoint.begin();
				iter!=setPoint.end(); ++iter)
			{
				delPoint(*iter);
			}
		};
		uint32 m_id;
	};
	
	typedef map<uint32, Player *> MapPlayer;
	MapPlayer g_mapPlayer;

	OneWayRelationBase * Player::getObjById( uint32 id ) const
	{
		MapPlayer::iterator iter=g_mapPlayer.find(id);
		if (iter==g_mapPlayer.end())
		{
			return NULL;
		}
		return iter->second;
	}

	bool Player::point( uint32 *p )
	{
		const SetPoint &setPoint = getSetPoint();
		for (uint32 i=0; i<10; ++i)
		{
			uint32 id = *(p+i);
			if (0 == id)
			{
				return true;
			}
			bool isFind= false;
			for(SetPoint::const_iterator iter = setPoint.begin();
				iter!=setPoint.end(); ++iter)
			{
				if (id == (*iter)->id())
				{
					isFind= true;
					break;
				}
			}
			return isFind;
		}
		assert(0);
		return false;
	}

	bool Player::bePoint( uint32 *p )
	{
		const SetPoint &setPoint = getSetBePoint();
		for (uint32 i=0; i<10; ++i)
		{
			uint32 id = *(p+i);
			if (0 == id)
			{
				return true;
			}
			bool isFind= false;
			for(SetPoint::const_iterator iter = setPoint.begin();
				iter!=setPoint.end(); ++iter)
			{
				if (id == (*iter)->id())
				{
					isFind= true;
					break;
				}
			}
			return isFind;
		}
		assert(0);
		return false;
	}


#define INIT_g_mapPlayer \
	g_mapPlayer.clear();\
	Player player[]={0,1,2,3,4};\
	g_mapPlayer.insert(make_pair(1,&player[1]));\
	g_mapPlayer.insert(make_pair(2,&player[2]));\
	g_mapPlayer.insert(make_pair(3,&player[3]));\
	g_mapPlayer.insert(make_pair(4,&player[4]));\

	void testRelation()
	{
		{
			INIT_g_mapPlayer
			{
				uint32 a[10]={1};
				assert(!player[1].point(a));
			}
			player[1].addPoint(&player[1]);
			{
				uint32 a[10]={1};
				assert(!player[1].point(a));
			}
			player[1].addPoint(&player[2]);
			player[1].addPoint(&player[2]);
			player[1].addPoint(&player[3]);
			player[1].addPoint(&player[4]);
			{
				uint32 a[10]={2,3,4};
				assert(player[1].point(a));
			}
			{
				uint32 a[10]={1};
				assert(player[2].bePoint(a));
			}
			{
				uint32 a[10]={1};
				assert(player[3].bePoint(a));
			}
			{
				uint32 a[10]={1};
				assert(player[4].bePoint(a));
			}
			//test serialize
			{

				uint8 buf[100];
				Memory out(buf, 100);
				const Memory in(buf, 100);

				for (uint32 i=0; i<ArrayLen(player); ++i)
				{
					player[i].serialize(out);
				}
				
				for (uint32 i=0; i<ArrayLen(player); ++i)
				{
					player[i].clear();
				}			

				for (int i=0; i<(int)ArrayLen(player); ++i)
				{
					player[i].unserialize(in);
					assert(!in.error());
				}
			}
			{
				uint32 a[10]={2,3,4};
				assert(player[1].point(a));
			}
			{
				uint32 a[10]={1};
				assert(player[2].bePoint(a));
			}
			{
				uint32 a[10]={1};
				assert(player[3].bePoint(a));
			}
			{
				uint32 a[10]={1};
				assert(player[4].bePoint(a));
			}




			player[1].delPoint(&player[4]);
			{
				uint32 a[10]={2,3};
				assert(player[1].point(a));
			}
			{
				uint32 a[10]={1};
				assert(!player[4].bePoint(a));
			}
			{
				uint32 a[10]={1};
				assert(player[2].bePoint(a));
			}
			{
				uint32 a[10]={1};
				assert(player[3].bePoint(a));
			}

			player[1].delPoint(&player[4]);
			player[1].delPoint(&player[3]);
			player[1].delPoint(&player[2]);
			player[1].delPoint(&player[1]);
			{
				uint32 a[10]={2};
				assert(!player[1].point(a));
			}
			{
				uint32 a[10]={1};
				assert(!player[4].bePoint(a));
			}
			{
				uint32 a[10]={1};
				assert(!player[3].bePoint(a));
			}
		}
	}
}//end namespace

void testFriend()
{

	testRelation();
	Info("done");
}