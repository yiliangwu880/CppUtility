/*
brief: 对随机数有操作：peer, use
	   peer在有限次数内可以重复。
	   use的随机数符合唯一性
	   插入式公共代码
使用例子：服务器随机名字
流程：
	//peer
		for(循环读取随机)
		{
			if (缓存不满)
			{
				随机新的一部分添加到缓存
			}
			循环从缓存中获取一个
		}
		
	//use
	删除被使用缓存随机数
*/

#pragma once

#include "../utility/typedef.h"
#include <set>
#include "random.h"

template<typename CreateName>
class RandomPeerUse
{
public:
	explicit RandomPeerUse(uint32 max_buf_len=100)
		:m_buf()
		,m_peerIter(m_buf.end()) 
		,MAX_BUF_LEN(max_buf_len)
		,m_createRandom(10)
	{
	};

	uint32 peer()
	{
		if (m_buf.size() < MAX_BUF_LEN)
		{
			bool bFirst = false;
			if (m_peerIter==m_buf.end()) //first initialize
			{
				bFirst = true;
			}
			uint32 num = MAX_BUF_LEN - m_buf.size();
			for(uint32 i=0; i<num; ++i)
			{
				m_buf.insert(m_createRandom.GetRandom());
			}
			if (bFirst)
			{
				m_peerIter = m_buf.begin();
			}
		}
		loopIter();
		assert(!m_buf.empty());
		return *m_peerIter;
	};

	bool use(uint32 r)
	{
		Set::iterator iter = m_buf.find(r);
		if (iter == m_buf.end())
		{
			return false;
		}
		if (m_peerIter == iter)
		{
			m_peerIter = m_buf.erase(iter);
		}
		else
		{
			m_buf.erase(iter);
		}
		return true;
	};

private:
	RandomPeerUse &operator=(const RandomPeerUse& r);
	inline void loopIter()
	{
		if (m_buf.empty())
		{
			m_peerIter = m_buf.begin();
			return;
		}
		if (m_peerIter==m_buf.end())
		{
			m_peerIter=m_buf.begin();
			return;
		}
		++m_peerIter;
		if (m_peerIter==m_buf.end())
		{
			m_peerIter=m_buf.begin();
		}
	}
private:
	typedef std::set<uint32> Set;

	Set m_buf;
	Set::iterator m_peerIter;		//peer使用的缓存指引
	const size_t MAX_BUF_LEN;
	CreateName m_createRandom;		
};


