
#pragma once
#include <list>


/**
@name : ��ת����
@brief: �ڶ��߳�������-������ģʽ���������߳��������Ӷ����������̴߳Ӷ���ȡ�߶���
        ��Ϊ�漰�����̷߳��ʣ�������б��������/ȡ�߶���ʱ���������ģ��ʵ�����ֶ��߳�
		��ȫ�Ķ��У�����ʹ���������з�ת�ļ��������Լ���ļ���������ļ���
*/


//����һ���̶߳�������߳�д
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
	//���߳̽ӿ�
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

	//д�߳̽ӿ�
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



