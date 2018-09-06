


#include "stdafx.h"


//#include "../gameUtility/res.h"
#include "../gameUtility/friend.h"
#include "../utility/misc.h"
#include "../utility/logFile.h"

using namespace std;
using namespace wyl;

namespace
{
	void t1()
    {

       // g_logFile.setShowLv(LOG_LV_ERROR);
        LOG_INFO("t");
        LOG_DEBUG("t%d", 1);
        LOG_WARN("t");
        LOG_ERROR("t%02d%s", 1, "s");
        LOG_FATAL("t%02d%s", 1, "s");
    }

}//end namespace

void testLogFile()
{

	t1();
	Info("done");
}