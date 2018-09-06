// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

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
//#include <Cctype>
#include <bitset>
#include <typeinfo>
#include <new>
#include <exception>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <valarray>
#include <ctime>




//for easy test
//////////////////////////////////////////////////////////////////////////
/* printElements()
 * - prints optional C-string optcstr followed by
 * - all elements of the collection coll
 * - separated by spaces
 */
template <typename T>
inline void printElements (const T& coll, const char* optcstr="")
{
    typename T::const_iterator pos;

    std::cout << optcstr;
    for (pos=coll.begin(); pos!=coll.end(); ++pos) {
        std::cout << *pos << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
void Info(T t)
{
	std::cout << t << std::endl;
}
//////////////////////////////////////////////////////////////////////////

#include "./utility/misc.h"
//#include "./utility/stlBoost.h"
#include "./utility/singleton.h"
#include "./utility/UniqueId.h"
#include "./time/cd.h"
#include "./time/sys_time.h"
#include "./time/alarm_clock.h"
#include "./Random/random.h"
#include "./Random/randomName.h"
#include "./utility/MultIndexContainer.h"
#include "./utility/logFile.h"
#include "./utility/global_reg.h"
#include "./utility/memoryStream.h"


#include <stdio.h>
#include <queue>
#include "config.h"




// TODO: 在此处引用程序需要的其他头文件
