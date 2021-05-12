#include "attrCal.h"

namespace su
{
	AttrsCal::AttrsCal(const Attrs &values)
		:m_values(values)
		, m_isBase(true)
	{
	}
	AttrsCal::AttrsCal(const AttrsCal &other)
	{
		m_values    = other.m_values;
		*m_sums     = *(other.m_sums);
		m_sumValues = other.m_sumValues;
		m_mul       = other.m_mul;
		m_isBase    = other.m_isBase;
	}

	AttrsCal & AttrsCal::operator=(const AttrsCal &other)
	{
		m_values    = other.m_values;
		*m_sums     = *(other.m_sums);
		m_sumValues = other.m_sumValues;
		m_mul       = other.m_mul;
		m_isBase    = other.m_isBase;
		return *this;
	}

	AttrsCal::AttrsCal(const Sums &sums, double mul /*= 1*/)
		:m_mul(mul)
	{
		*m_sums = sums;
		for (auto &v : *m_sums)
		{
			AttrsCal &attrCal = v.second;
			for (auto &attrV : attrCal.Values())
			{
				m_values[attrV.first] += attrV.second;
			}
		}
		if (1 != m_mul)
		{
			m_sumValues = m_values;
			for (auto &attrV : m_values)
			{
				m_values[attrV.first] = (int32_t)(m_values[attrV.first] * m_mul);
			}
		}
	}

	Attrs::mapped_type AttrsCal::GetAttr(Attr attr) const
	{
		auto it = m_values.find((Attrs::key_type)attr);
		if (it == m_values.end())
		{
			L_ERROR("find attr fail %d", (Attrs::key_type)attr);
			return 0;
		}
		return it->second;
	}

	void AttrsCal::Change(const AttrsCal &c, deque<SubSys> keys)
	{
		if (keys.empty())
		{
			if (!m_isBase)
			{
				L_ERROR("change c is not base");
				return;
			}
			m_values = c.Values();
			return;
		}
		auto it = m_sums->find(keys.front());
		if (it == m_sums->end())
		{
			L_ERROR("can't find keys %d", keys.front());
			return;
		}
		AttrsCal &attrCal = it->second; //没有就认为0值处理

		Attrs oldValues = attrCal.Values();
		keys.pop_front();
		attrCal.Change(c, keys);
		Attrs newValues = attrCal.Values();


		Attrs *pValues = (1 == m_mul) ? &m_values : &m_sumValues;

		std::set<Attrs::key_type> attrKeys;
		for (auto &attrV : oldValues)
		{
			auto &v = (*pValues)[attrV.first];
			v = v - oldValues[attrV.first];
			attrKeys.insert(attrV.first);
		}
		for (auto &attrV : newValues)
		{
			auto &v = (*pValues)[attrV.first];
			v = v + newValues[attrV.first];
			attrKeys.insert(attrV.first);
		}
		if (1 != m_mul)
		{
			for (auto &attrKey : attrKeys)
			{
				m_values[attrKey] = (int32_t)(m_sumValues[attrKey] * m_mul);
			}
		}
	}

}