//预编译头
// 或是经常使用但不常更改的

#pragma once

#include <typeinfo>
#include <memory.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <list>
#include <memory>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <math.h>
#include <set>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iosfwd>
#include <iomanip>
#include <functional>
#include <limits>
#include <deque>
#include <bitset>
#include <typeinfo>
#include <new>
#include <exception>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <valarray>
#include <ctime>
#include <stdio.h>
#include <queue>



//////////////////////////////////////////////////////////////////////////

#include "./utility/misc.h"
#include "./utility/stlBoost.h"
#include "./utility/UniqueId.h"
#include "./time/cd.h"
#include "./time/sys_time.h"
#include "./time/alarm_clock.h"
#include "./Random/random.h"
#include "./Random/randomName.h"
#include "./utility/MultIndexContainer.h"
#include "./utility/log_def.h"
#include "./utility/global_reg.h"
#include "./utility/memoryStream.h"
#include "./utility/log_def.h"





//for easy test
//////////////////////////////////////////////////////////////////////////

template <typename T>
void Info(T t)
{
	std::stringstream ss;
	ss << t;
	LogMgr::Obj().Printf(LL_INFO, __FILE__, __LINE__, __FUNCTION__, ss.str().c_str());
}

