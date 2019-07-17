


#include "stdafx.h"


//#include "../gameUtility/res.h"
#include "../gameUtility/friend.h"
#include "../utility/misc.h"
#include "../utility/log_def.h"

using namespace std;
using namespace wyl;

namespace
{
	void t1()
    {

       // g_logFile.setShowLv(LOG_LV_ERROR);
        L_INFO("t");
        L_DEBUG("t%d", 1);
        L_WARN("t");
        L_ERROR("ok!. this line print error is ok. t%02d%s", 1, "s");
        L_FATAL("ok!. this line print error is ok. t%02d%s", 1, "s");
    }

}//end namespace

void testLogFile()
{

	t1();
	Info("done");
}