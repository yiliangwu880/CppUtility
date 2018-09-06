/*
brief: use example and test code
*/
#include "debugLog.h"
#include <algorithm>
#include <iostream>

using namespace std;

class ConcreteLog : public DebugLogStdOut
{
public:
	~ConcreteLog(){}

private:
	virtual void printLog(const string &s)
	{
		cout << s;
	}

};

ConcreteLog gLog;
void TestLog()
{
	gLog << 3 << "hello" ;
}

DebugLogStdOut::~DebugLogStdOut()
{

}
