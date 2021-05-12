/*
解决问题：游戏人物属性计算，通常多个子系统组合统计起来。代码组织不好很容易改一个子系统属性，导致全部子系统都计算一遍。效率慢。
设计目的：对只改一个子系统属性，尽量少计算的原则，又要兼顾容易使用。
代码模板的方式提供使用，不同项目需要修改。

author:yiliangwu880
	you can get more refer from https ://github.com/yiliangwu880/CppUtility.git
*/
#include <functional>
#include <limits>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>  
#include <type_traits>
#include <memory>
#include <array>
#include <set>

using namespace std;

using namespace placeholders;

namespace su
{
	//子系统类型，例如：装备系统，等级系统， 。。。。
	enum class SubSys
	{
		S1,
		S2,
		S12, //属性为 （S1+S2）*50%
		RIDE //坐骑系统属性，*50% 后，再加到人物上
	};
	//属性类型
	enum class Attr
	{
		pAt, //物理攻击
		mAt,//法术攻击
		pAtPer, //物理攻击万分比
		mAtPer,
		Max
	};
	struct EnumHashFun
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	class AttrsCal;
	using Attrs = std::unordered_map<Attr, int32_t, EnumHashFun>;
	//using Sums = std::unordered_map<SubSys, AttrsCal, EnumHashFun>;

	//代表N个子系统累加属性的表达式
	class AttrsCal
	{
		using Sums = std::unordered_map<SubSys, AttrsCal, EnumHashFun>;
		Attrs m_values;			        //表达式的结果值。 例如： values =  (sums[x]+sums[x])*mul, 或者 表达最基础子系统值时。没有sums
		std::unique_ptr<std::unordered_map<SubSys, AttrsCal, EnumHashFun>> m_sums	//表达式的 累加列表
		Attrs m_sumValues;              //m_sums的累加结果。 if 1 != m_mul 才使用. m_sumValues= (sums[x]+sums[x])
		double m_mul = 1;               //需不需要？待考虑
		bool m_isBase = false;          //true表示最基础子系统值，只有 m_values有用，不需要计算。
	public:
		AttrsCal(const Attrs &values)
			:m_values(values)
			, m_isBase(true)
		{
			m_sums = std::make_unique<std::unordered_map<SubSys, AttrsCal, EnumHashFun>>();
		}
		AttrsCal(const Sums &sums, double mul = 1)
			:m_mul(mul)
		{
			m_sums = std::make_unique<std::unordered_map<SubSys, AttrsCal, EnumHashFun>>();
			*m_sums = sums;
			for (auto &v : m_sums)
			{
				std::shared_ptr<AttrsCal> pC = v.second;
				for (auto &attrV : pC->Values())
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
#if 0
		const Attrs &Values() const
		{
			return m_values;
		}
		Attrs::mapped_type GetAttr(Attr attr) const
		{
			auto it = m_values.find((Attrs::key_type)attr);
			if (it == m_values.end())
			{
				UNIT_ERROR("find attr fail %d", (Attrs::key_type)attr);
				return 0;
			}
			return it->second;
		}

		//@keys 代表c在this对象中，所属得位置。 
		/*例如：
		S12 = (S1+S2)
		this = RIDE*50% + S12*50%
		如果c时S1。那么keys={S12,S1}

		用单个属性表达函数流程
		oldV = m_sums[k1]
		m_sums[k1].change(k2, c)  --支持多层嵌套，递归到最基础值
		m_sumValues = m_sumValues - oldV 、
		m_Values = m_sumValues*mul
		*/
		void Change(const AttrsCal &c, deque<Sums::key_type> keys)
		{
			if (keys.empty())
			{
				if (!m_isBase)
				{
					UNIT_ERROR("change c is not base");
					return;
				}
				m_values = c.Values();
				return;
			}
			auto it = m_sums->find(keys.front());
			if (it == m_sums->end())
			{
				UNIT_ERROR("can't find keys %d", keys.front());
				return;
			}
			AttrsCal &pC = it->second; //没有就认为0值处理

			Attrs oldValues = pC.Values();
			keys.pop_front();
			pC.Change(c, keys);
			Attrs newValues = pC.Values();


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
#endif
	};
}

