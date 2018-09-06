/************************************************************************/
/*                                                                      
brief:
author:YiliangWu 
resource base code
*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"


struct Money
{
	Money():num(0){}
	bool enableDec(uint32 decNum) const
	{
		if (num>=decNum)
		{
			return true;
		}
		return false;
	}
	void dec(uint32 decNum)
	{
		if (num>=decNum)
		{
			num-=decNum;
		}
	}
	void add(uint32 addNum)
	{
		uint32 t = num;
		t+=addNum;
		if (t>num)
		{
			num = t;
		}
	}
	uint32 num;
};

