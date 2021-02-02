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

#include "misc.h"
#include "easy_code.h"
#include "stl_boost.h"
#include "./Random/random.h"
#include "./Random/randomName.h"
#include "log_def.h"





//for easy test
//////////////////////////////////////////////////////////////////////////

template <typename T>
void Info(T t)
{
	std::stringstream ss;
	ss << t;
	L_INFO("%s", ss.str().c_str());
}

