

#include "expAndLvup.h"

void LvupBase::addExp( uint32 addNum )
{
	uint32 max_lv = getMaxLv();
	if (m_lv >= max_lv)
	{
		return;
	}
	m_exp+=addNum;

	uint32 needExp = getNextLvNeedExp(m_lv);
	while (m_exp >= needExp)
	{
		m_exp -= needExp;
		m_lv++;
		onLvup();
		if (m_lv >= max_lv)
		{
			break;
		}
		needExp = getNextLvNeedExp(m_lv);
	};
}
