/************************************************************************/
/*
brief: 消耗经验升级
author:YiliangWu

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"


class LvupBase
{
protected:
	uint32 m_lv = 0;
	uint32 m_exp = 0;   //经验，升级会消耗掉一定数量。

public:
	void AddExp(uint32 addNum);

	virtual uint32 GetNextLvNeedExp(uint32 currentLv) const =0;
	virtual uint32 GetMaxLv() const =0;

private:
	virtual uint32 OnLvup()=0;
};

