
#include "../utility/UniqueId.h"
#include "../utility/memoryStream.h"
#include "../utility/stlBoost.h"


using namespace std;
using namespace wyl;

namespace
{
	UniqueID g_unique;
	set<uint32> set_id;
	vector<uint32> vec_id;
	void test1()
	{
		for (uint32 i=0; i<90000; ++i)
		{
			int r = rand()%2;
			if (1 == r)
			{
				uint32 id = g_unique.newId();
				if (id==0)
				{
					continue;
				}
				assert(set_id.insert(id).second);
				vec_id.push_back(id);
			}
			else
			{	//del id
				if (0 == set_id.size())
				{
					continue;
				}
				int index = rand()%vec_id.size();
				assert(1 == set_id.erase(vec_id[index]));
				g_unique.delId(vec_id[index]);
				VecRemoveByIndex(vec_id, index);
			}
		}
    }
    void test2()
    {
        UniqueID g_unique;
        g_unique.delId(0);
        assert(1 == g_unique.newId());
        g_unique.delId(2);
        assert(2 == g_unique.newId());
    }

}//namespace{



void testUniquId()
{
	test1();
    test2();
	Info("testUniquId done");
}