
#include "config.h"
#include "../utility/circleBuf.h"
#include "../utility/logFile.h"
#include "../utility/misc.h"
#include <string>
#include <vector>
#include "../utility/easyTest.h"



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