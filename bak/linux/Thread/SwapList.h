
#pragma once
#include <list>


/**
@name : 翻转队列
@brief: 在多线程生产者-消费者模式中生产者线程向队列添加对象，消费者线程从队列取走对象
        因为涉及到多线程访问，这个队列必须在添加/取走对象时加锁。这个模版实现这种多线程
		安全的队列，另外使用两个队列翻转的技术，可以极大的减少锁互斥的几率
*/


//合适一个线程读，多个线程写
template<typename T, uint32 MAX_SIZE, typename Mutex>
class SwapBuf
{
public:
	SwapBuf()
		:m_p_read(&m_buf1)
		,m_p_write(&m_buf2)
	{
	}
protected:
	//读线程接口
	//////////////////////////////////////////////////////////////////////////
	uint32 enableReadLen() const
	{
		return m_p_read->len();
	}

	bool read(T *t, uint32 len)
	{
		if ( m_p_read->empty() )
		{
			Guard<Mutex> guard(m_mutex);
			if ( m_p_write->empty() )
			{
				return false;
			}
			std::swap(m_p_write, m_p_read);
		}

		return m_p_read->read(t, len);
	}

	T pop() 
	{
		if ( m_p_read->empty() )
		{
			Guard<Mutex> guard(m_mutex);
			if ( m_p_write->empty() )
			{
				return false;
			}
			std::swap(m_p_write, m_p_read);
		}
		return m_p_read->pop();
	}
	//////////////////////////////////////////////////////////////////////////

	//写线程接口
	//////////////////////////////////////////////////////////////////////////
	uint32 enableWriteLen() const
	{
		Guard<Mutex> guard(m_mutex);
		return m_p_write->emptyLen();
	}

	bool write(const T *t, uint32 len)
	{
		Guard<Mutex> guard(m_mutex);
		return m_p_write->write(t, len);
	}
	void push(const T &data)
	{
		Guard<Mutex> guard(m_mutex);
		m_p_write->push(data);
	}
	//////////////////////////////////////////////////////////////////////////
protected:

protected:
	typedef CircleBuf<T, MAX_SIZE/2> Buf;

	Mutex m_mutex;

	Buf	m_buf1,m_buf2;
	Buf *m_p_read, *m_p_write;
};

 // End of file



