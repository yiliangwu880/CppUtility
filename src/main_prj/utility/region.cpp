/*
brief:区间查找功能

*/
#include "region.h"
#include "misc.h"
#include <assert.h>
#include <algorithm>


bool MapRegion::addRegion( uint32 lower, uint32 upper )
{
	if ( lower > upper)
	{
		return false;
	}
	//检查已经存在包含 lower或upper 的情况
	Region region;
	if(find(lower, region)
		||find(upper, region)
		)
	{
		return false;
	}
	//空容器的情况
	LowMapUp::const_iterator it = m_map_region.lower_bound(lower);
	if (it == m_map_region.end())
	{
		m_map_region.insert(std::make_pair(lower, upper));
		return true;
	}
	//检查跨越后一区间
	if (upper >= it->first)
	{
		return false;
	}

	m_map_region.insert(std::make_pair(lower, upper));
	return true;
}

bool MapRegion::find( uint32 num, Region &region ) const
{
	region.first = 0;
	region.second = 0;
	LowMapUp::const_iterator it = m_map_region.lower_bound(num);
	if (it != m_map_region.end())
	{
		if (num == it->first)
		{
			region = *it;
			return true;
		}
	}

	if (it == m_map_region.begin())
	{
		return false;
	}

	--it;
	if (it->second < num)
	{
		return false;
	}

	region = *it;
	return true;
}

bool MapRegion::erase( uint32 lower )
{
	LowMapUp::iterator it = m_map_region.lower_bound(lower);
	if (it == m_map_region.end())
	{
		return false;
	}
	m_map_region.erase(it);
	return true;
}

void MapRegion::clear()
{
	m_map_region.clear();
}

void MapRegion::getSpan( uint32 lower, uint32 upper, VecSpan &vec_span ) const
{
	vec_span.clear();
	if (lower>=upper)
	{
		return;
	}
	uint32 lastOldRank = upper;
	for (uint32 i=0; i<1000; ++i)
	{
		if (lower>=lastOldRank)
		{
			break;
		}
		Region region;
		if(!find(lastOldRank, region))
		{//找不到区间,往小的区间找
			LowMapUp::const_iterator it = m_map_region.lower_bound(lastOldRank);
			if (it == m_map_region.begin())
			{
				break;
			}
			--it;
			lastOldRank = it->second; 
			continue;
		}

		if (region.first <= lower)
		{//范围在一个区间内

			Span d;
			d.lower = region.first;
			d.dist = lastOldRank-lower;
			vec_span.push_back(d);
			break;
		}

		//范围在多个区间内
		Span d;
		d.lower = region.first;
		d.dist = lastOldRank-(region.first-1);//临近区间间提升一名次，按高名次区间计算。 比如 [1,10][11,20].  10~11的跨度算在 [11,20区间]
		vec_span.push_back(d);

		lastOldRank = region.first-1; 
	}
}

bool OverlapRegion::addRegion( uint32 start, uint32 end )
{
	if (findRegion(start, end))
	{
		return false;
	}

	//保存和端点重叠的区间
	VecLine start_o_l, end_o_l;
	findOverLap(start, start_o_l);
	findOverLap(end, end_o_l);
	Line my_line;
	my_line.start = start;
	my_line.end = end;

	//端点没有就插入新的
	{
		PointMapLines::iterator point_it = m_point_m_lines.find(start);
		if (point_it == m_point_m_lines.end())
		{
			point_it = m_point_m_lines.insert(make_pair(start, Lines())).first;
			CONST_FOR_IT(VecLine, start_o_l)
			{
				point_it->second.push_back(*it);
			}
		}
		point_it->second.push_back(my_line);
	}	
	{
		PointMapLines::iterator point_it = m_point_m_lines.find(end);
		if (point_it == m_point_m_lines.end())
		{
			point_it = m_point_m_lines.insert(make_pair(end, Lines())).first;
			CONST_FOR_IT(VecLine, end_o_l)
			{
				point_it->second.push_back(*it);
			}
		}
		point_it->second.push_back(my_line);
	}

	//构造端点之间所有端点的信息
	{
		PointMapLines::iterator start_it = m_point_m_lines.upper_bound(start);
		PointMapLines::iterator end_it = m_point_m_lines.find(end);
		for (; start_it!=end_it; ++start_it)
		{
			Lines &lines = start_it->second;
			lines.push_back(my_line);
		}
	}
	return true;
}

void OverlapRegion::findOverLap( uint32 point, VecLine &vec_line ) const
{
	vec_line.clear();
	//找右端点 向左的线条 加入vecLine
	//找最小>=point的点
	PointMapLines::const_iterator point_it = m_point_m_lines.lower_bound(point);
	if (point_it == m_point_m_lines.end()) //找不到
	{
		return;
	}

	CONST_FOR_IT(Lines, point_it->second)
	{
		if (it->start <= point)
		{
			vec_line.push_back(*it);
		}
	}
}

void OverlapRegion::findOverLap( uint32 start, uint32 end, VecLine &vec_line ) const
{
	vec_line.clear();	

	if (start > end)
	{
		return;
	}
	if (start == end)
	{
		findOverLap(start, vec_line);
		return;
	}
	//找右端点 向左的线条 加入vecLine
	//找最小>=point的点
	{
		uint32 point = end;
		PointMapLines::const_iterator point_it = m_point_m_lines.lower_bound(point);
		if (point_it != m_point_m_lines.end())
		{
			CONST_FOR_IT(Lines, point_it->second)
			{
				if (it->start <= point)
				{
					vec_line.push_back(*it);
				}
			}	
		}

	}

	//找右端点 向左的线条 加入vecLine
	{
		uint32 point = start;
		PointMapLines::const_iterator point_it = m_point_m_lines.lower_bound(point);
		if (point_it != m_point_m_lines.end())
		{
			CONST_FOR_IT(Lines, point_it->second)
			{
				if (it->start <= point)
				{
					vec_line.push_back(*it);
				}
			}	
		}
	}

	//找start,end之内的区间
	{
		PointMapLines::const_iterator start_it = m_point_m_lines.upper_bound(start);
		PointMapLines::const_iterator end_it = m_point_m_lines.lower_bound(end);
		for (; start_it!=end_it; ++start_it)
		{
			CONST_FOR_IT(Lines, start_it->second)
			{
				if (it->start > start && it->end < end)
				{
					vec_line.push_back(*it);
				}
			}			
		}
	}
	
	//删除重复
	struct SortFun
	{
		bool operator()(const Line &left, const Line &right)
		{
			if (left.start == right.start)
			{
				return left.end < right.end;
			}
			else
			{
				return left.start < right.start;
			}
		}
	};
	SortFun sort_f;

	struct Fun
	{
		bool operator()(const Line &left, const Line &right)
		{
			if (left.start == right.start && left.end == right.end)
			{
				return true;
			}
			return false;
		}
	};
	Fun f;
	sort(vec_line.begin(), vec_line.end(), sort_f);
	VecLine::iterator ret = unique(vec_line.begin(), vec_line.end(), f);
	vec_line.erase(ret, vec_line.end());


}

bool OverlapRegion::erase( uint32 start, uint32 end )
{
	if(!findRegion(start, end))
	{
		return false;
	}

	//assert success
	PointMapLines::iterator point_it = m_point_m_lines.find(start); 
	for (;point_it!=m_point_m_lines.end();)
	{
		//删除点保存的线
		Lines &lines = point_it->second;
		FOR_IT(Lines, lines)
		{
			if (it->start == start && it->end == end)
			{
				lines.erase(it);
				break;
			}
		}

		if (point_it->first == start && isEmptyPoint(point_it))
		{
			m_point_m_lines.erase(point_it++);
		} 
		else if (point_it->first == end && isEmptyPoint(point_it))
		{
			m_point_m_lines.erase(point_it);
			return true;
		}
		else
		{
			++point_it;
		}
	}
	assert(false);
	return false;
}

void OverlapRegion::clear()
{
	m_point_m_lines.clear();
}

bool OverlapRegion::findRegion( uint32 start, uint32 end ) const
{
	PointMapLines::const_iterator point_it = m_point_m_lines.find(start);
	if (point_it == m_point_m_lines.end())
	{
		return false;
	}
	const Lines &lines = point_it->second;

	CONST_FOR_IT(Lines, lines)
	{
		if (it->start == start && it->end == end)
		{
			return true;
		}
	}
	return false;
}

bool OverlapRegion::isEmptyPoint(const  PointMapLines::iterator &point_it) const
{
	const Lines &lines = point_it->second;
	uint32 point = point_it->first;

	CONST_FOR_IT(Lines, lines)
	{
		if (it->start == point || it->end == point )
		{
			return false;
		}
	}
	return true;
}
