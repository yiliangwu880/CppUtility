/************************************************************************/
/*                
供参考
brief:
author:YiliangWu 
experience and lvup base code
改成成这样：

//消耗经验升级组件. 无脑重写几个函数，就可以无BUG升级
public abstract class LvUpCom
{
public abstract uint GetLv();
public abstract void SetLv(uint lv);
public abstract uint GetExp();
public abstract void SetExp(uint exp);
public abstract uint GetMaxLv();
public abstract uint GetNextLvNeedExp();

public virtual void OnLvup(uint new_lv) { }
public void AddExp(uint addNum)
{
uint max_lv = GetMaxLv();
uint lv = GetLv();
uint exp = GetExp();
if (lv >= max_lv)
{
return;
}
exp += addNum; SetExp(exp);

uint needExp = GetNextLvNeedExp();
while (exp >= needExp)
{
exp -= needExp; SetExp(exp);
lv++; SetLv(lv);
OnLvup(lv);
if (lv >= max_lv)
{
break;
}
needExp = GetNextLvNeedExp();
};
}
}
*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"


class LvupBase
{
public:
	LvupBase():m_lv(0),m_exp(0){}
	void addExp(uint32 addNum);
protected:
	virtual uint32 getNextLvNeedExp(uint32 currentLv) const =0;
	virtual uint32 getMaxLv() const =0;
private:
	virtual uint32 onLvup()=0;
protected:
	uint32 m_lv;
	uint32 m_exp;   //经验，升级会消耗掉一定数量。
};

