/*
brief: common log
*/

#pragma once
//#include "stdafx.h"
#include <string>
#include <sstream>

class DebugLogStdOut
{
public:
	virtual ~DebugLogStdOut()=0;

	template<typename T>
	DebugLogStdOut & operator<< (const T &t)
	{
		m_s << t;
		printLog(m_s.str());
		m_s.clear();
		return *this;
	}

private:
	virtual void printLog(const std::string &s)=0;
private:

	std::stringstream m_s;
};