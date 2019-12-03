

#include "expAndLvup.h"

void LvupBase::AddExp( uint32 addNum )
{
	uint32 max_lv = GetMaxLv();
	if (m_lv >= max_lv)
	{
		return;
	}
	m_exp+=addNum;

	uint32 needExp = GetNextLvNeedExp(m_lv);
	while (m_exp >= needExp)
	{
		m_exp -= needExp;
		m_lv++;
		OnLvup();
		if (m_lv >= max_lv)
		{
			break;
		}
		needExp = GetNextLvNeedExp(m_lv);
	};
}
