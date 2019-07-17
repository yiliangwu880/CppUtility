
#include "../utility/circleBuf.h"
#include "../utility/log_def.h"
#include "../utility/misc.h"
#include <string>
#include <vector>



using namespace std;


void test_misc()
{
    vector<int> v;
    v.push_back(1);
    v.push_back(2);

    FOR_IT_(v)
    {
        static int i=0;
        if (0==i)
        {
            assert(1==*it);
        }
        if (1==i)
        {
            assert(2==*it);
        }
            ++i;
    }
}