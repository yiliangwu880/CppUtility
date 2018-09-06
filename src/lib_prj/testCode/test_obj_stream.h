/*
brief: 动态对象的流化 功能
*/

#pragma once

#include "../utility/obj_stream.h"


class SubStreamObj_1 : public StreamObj
{
public:
	std::string m_name;
protected:
	virtual bool doRead(Memory &out) const;
	virtual bool doWrite(const Memory &in);
};

class SubStreamObj_2 : public StreamObj
{
public:
	std::string m_name;
protected:
	virtual bool doRead(Memory &out) const;
	virtual bool doWrite(const Memory &in);
};

class SubStreamObj_3 : public SubStreamObj_2
{
public:
	std::string m_o3_name;
protected:
	virtual bool doRead(Memory &out) const;
	virtual bool doWrite(const Memory &in);
};

