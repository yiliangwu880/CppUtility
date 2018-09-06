/*
author: YiliangWu
*/

#pragma once

#include "typedef.h"
#include <iostream>
#include "string.h"
#include <sys/socket.h>

//�������У�֧��һ���߳�д����һ���̶߳���
//para:typename T, Ԫ������
//para:uint32 MAX_SIZE, ������Ԫ�ظ���
template<typename T, uint32 MAX_SIZE>
class CircleBuf
{
	const static uint32 ARRAY_LEN = MAX_SIZE+1; //������һ����λ�ò��ã��������������߿�. m_r_index==m_w_index ��ʾ��, �����ڴ�ʣһ��Ԫ�ؿռ�Ϊ��

public:
	typedef T ValueType;
public:
	CircleBuf()
	{
		memset(m_buf, 0, sizeof(m_buf));
		m_r_idx=m_w_idx=0;
	}

    bool full() const
    {
        if (next(m_w_idx)==m_r_idx)
        {
            return true;
        }
        return false;
    }

    bool empty() const
    {
        if (m_w_idx==m_r_idx)
        {
            return true;
        }
        return false;
    }

    //���������û�޸�
    bool push(const T &data)
    {
        if (full())
        {
            return false;
        }
        m_buf[m_w_idx] = data;
        m_w_idx = next(m_w_idx);
        return true;
    }

    bool pop(T &data) 
    {
        if (empty())
        {
			data=T();
            return false;
        }
		uint32 index = m_r_idx;
		data = m_buf[index];
        m_r_idx=next(m_r_idx);
		return true;
    }


    //ӵ��Ԫ������
    uint32 len() const
    {
		uint32 w_idx = m_w_idx;
		uint32 r_idx = m_r_idx;
		if (w_idx >= r_idx)
        {
			return w_idx - r_idx;
        }
        else
        {
			return ARRAY_LEN + (w_idx - r_idx);
        }
    }

    //freeԪ������
    uint32 freeLen() const
    {
		return MAX_SIZE - len();
    }
	
	//���������ڴ棬ʧ�ܲ����κ��޸�
	bool write(const T *t, uint32 _len)
	{
		if (0==_len || _len>freeLen())
		{
			return false;
		}
		uint32 r_idx = m_r_idx;
		uint32 w_idx = m_w_idx;

		uint32 to_end_len = ARRAY_LEN - w_idx;
		if (r_idx>w_idx || _len <= to_end_len)
		{  //���ֶ�copy�����
			memcpy((void *)&m_buf[w_idx], (const void *)t, sizeof(T)*_len);
		}
		else
		{
			//case _len > to_end_len
			memcpy((void *)&m_buf[w_idx], (const void *)t, sizeof(T)*to_end_len);
			//write to r-1
			memcpy((void *)&m_buf[0], (const void *)(t+to_end_len), sizeof(T)*(_len-to_end_len));
		}
		offsetWriteIdx(_len);
		return true;
	}

    //�ɹ����Ƕ�����ʧ��û�仯��
    //para: uint32 _len Ϊ *tָ������鳤��
    bool read(T *t, uint32 _len)
    {
        if (0 == _len || _len>len())
        {
            return false;
        }

		uint32 r_idx = m_r_idx;
		uint32 w_idx = m_w_idx;
		uint32 to_end_len = ARRAY_LEN - r_idx;
		if (w_idx >= r_idx || _len <= to_end_len)
        { //���ֶ�copy�����
			memcpy((void *)t, (const void *)&m_buf[r_idx], sizeof(T)*_len);
        }
		else
        {
			//case _len > to_end_len
			//read to array end 
			memcpy((void *)t, (const void *)&m_buf[r_idx], sizeof(T)*to_end_len);
			//read to r-1
			memcpy((void *)(t + to_end_len), (const void *)&m_buf[0], sizeof(T)*(_len - to_end_len));
		}
		offsetRead(_len);
        return true;
    }

	//��ȡ�������ڴ棬��Ϊ�Ѿ�д��Ļ��档Ȼ����������ɺ���д���������
    //ע�⣬�ýӿڱ�¶��Ա��ַ���û�����Ҫ�û�ȷ��������ȷ��������δ���塣
    //�Ľӿ�Ŀ���Ǹ�recv�Ⱥ�����Ч��ֱ��copy�ڴ棬�����м��copyһ��
	/*
		ʹ�����ӣ�
		uint32 len = 0;
		T *p=obj.getWillWrite(len);
        if(p != NULL)
        {
            int ret = ::recv(socket, (uint8 *)p, len*sizeof(ValueType), 0);
            if (0 == ret || -1 == ret)
            {
            return ret;
            }
            buf.finishWillWrite(ret);
        }
	*/
    //ʧ�ܷ���NULL,�ɹ����� _len ���ȵ���Ч�ڴ��ַ
	ValueType *getCanContinueWrite(uint32 &_len)
	{
        _len=enableContinuousWriteLen();
		if (0 == _len)
		{
			return NULL;
		}
		return &m_buf[m_w_idx];
	}

    //��ȡ�����Ķ����Ļ����ڴ��ַ����ȡ��ɺ��ٵ�����ɶ������ݡ�
    //ע�⣬�ýӿڱ�¶��Ա��ַ���û�����Ҫ�û�ȷ��������ȷ��������δ���塣
    //�Ľӿ�Ŀ���Ǹ�recv�Ⱥ�����Ч��ֱ��copy�ڴ棬�����м��copyһ��
	/*
		ʹ�����ӣ�
		uint32 len = 0;
		T *p=obj.getWillRead(len);
        if(p != NULL)
        {
		    memcpy(xx,xx,xx);
		    obj.finishWillRead(len);
        }
	*/
    //ʧ�ܷ���NULL,�ɹ����� _len ���ȵ���Ч�ڴ��ַ
    //uint32 &_len, ���ؿ�������д�ĳ���
	ValueType *getCanContinueRead(uint32 &_len)
	{
        _len = enableContinuousReadLen();
		if (0 == _len)
		{
			return NULL;
		}
		return &m_buf[m_r_idx];
	}

    //uint32 len, Ԫ������
    bool finishWillWrite(uint32 len)
    {
        if (len>enableContinuousWriteLen())
        {
            return false;
        }
       offsetWriteIdx(len);
       return true;
    }


    //Ԥ�ȶ�ȡ�����ı����
    //�ɹ����Ƕ�����ʧ��û�仯��
    //para: uint32 _len Ϊ *tָ������鳤��
    bool peer(T *t, uint32 _len) const
    {
        if (0 == _len || _len>len())
        {
            return false;
        }

		uint32 r_idx = m_r_idx;
		uint32 w_idx = m_w_idx;

		uint32 to_end_len = ARRAY_LEN - r_idx;
		if (w_idx >= r_idx || _len <= to_end_len)
        { //���ֶ�copy�����
			memcpy((void *)t, (const void *)&m_buf[r_idx], sizeof(T)*_len);
        }
		else
		{
			//case _len > to_end_len
			//read to array end 
			memcpy((void *)t, (const void *)&m_buf[r_idx], sizeof(T)*to_end_len);
			//read to r-1
			memcpy((void *)(t + to_end_len), (const void *)&m_buf[0], sizeof(T)*(_len - to_end_len));
		}
		return true;
    }

    bool finishWillRead(uint32 len)
    {
        if (len>enableContinuousReadLen())
        {
            return false;
        }
        offsetRead(len);
        return true;
    }



private:
    uint32 enableContinuousReadLen() const
    {
        if (empty())
        {
            return 0;
        }

		uint32 r_idx = m_r_idx;
		uint32 w_idx = m_w_idx; //ȡ˲�丱������

		if (w_idx >= r_idx)
        { //���ֶ�copy�����
			return w_idx - r_idx;
        }

		uint32 to_end_len = ARRAY_LEN - r_idx;
        return to_end_len;
    }
    uint32 enableContinuousWriteLen() const
    {
        if (full())
        {
            return 0;
		}
		uint32 r_idx = m_r_idx;
		uint32 w_idx = m_w_idx;

		if (r_idx>w_idx)
        {  //���ֶ�copy�����
			return r_idx - w_idx - 1;
        }

		uint32 to_end_len = ARRAY_LEN - w_idx;
		if (0 == r_idx)
        {
            to_end_len--;
        }
        return to_end_len;
    }
	uint32 next(const uint32 current) const
	{
		return (current+1)%ARRAY_LEN;
	}

    void offsetRead(uint32 len)
    {
        m_r_idx+=len;
        m_r_idx%=ARRAY_LEN;
    }

    void offsetWriteIdx(uint32 len)
    {
        m_w_idx+=len;
        m_w_idx%=ARRAY_LEN;
    }

private:
	uint32 m_r_idx; //ָ����һ����Ҫ��������
	uint32 m_w_idx; //ָ����һ����Ҫд������
	T m_buf[ARRAY_LEN];
};


struct CirMsgQueMem
{
	uint32 m_r_idx;		//ָ����һ����Ҫ��������
	uint32 m_w_idx;		//ָ����һ����Ҫд������
	uint32 m_buf_size;	//m_buf ���鳤�ȣ� ʵ����д��󳤶���1. Ŀ���������������߿�. m_r_index==m_w_index ��ʾ��, �����ڴ�һ���ֽ�Ϊ��
	char m_buf[1];		//�ɱ䳤������
};

//@brief ѭ�����ж�д��֧��һ���߳�����д����һ���߳�������
//	addr. circle message queue write
class CirMsgQue
{
public:

	//@para use_mem Ҫ�������ڴ棨�����ǹ����ڴ棩
	//@para len use_mem��Ч�ֽ���
	//ע�⣺ len���ȱ���>=sizeof(CirMsgQueMem) 
	static bool InitMemory(char *use_mem, uint32 len);

	CirMsgQue();

	//@para use_mem Ҫ�������ڴ棨�����ǹ����ڴ棩
	//@para len use_mem��Ч�ֽ���
	//ע�⣺ use_mem ������ InitMemory ��ʼ������Ȼʧ��
	bool Init(char *use_mem, uint32 len);

	bool Write(const char* buf, size_t len);
	
	//@para buf ����д���ַ
	//@para len buf��Ч�ռ��ֽ���
	//@return ���ض�ȡ���ȣ�0��ʾʧ��
	uint32 Read(char* buf, uint32 len);

	//���� Write ������д�ĳ��� 
	uint32 CanWriteLen() const
	{
		uint32 free_len = freeLen(); 
		if (free_len >= sizeof(uint32))
		{
			return free_len - sizeof(uint32);
		}
		else
		{
			return 0;
		}
	}

	//��д�ֽ�����
	uint32 len() const;

	//δд�ֽ�����
	//Write����ʵ�������ֽ���=Write���󳤶�+sizeof(uint32))
	uint32 freeLen() const;

	bool full() const
	{
		if (next(m_mem->m_w_idx) == m_mem->m_r_idx)
		{
			return true;
		}
		return false;
	}

	bool empty() const
	{
		if (m_mem->m_w_idx == m_mem->m_r_idx)
		{
			return true;
		}
		return false;
	}



	void offsetRead(uint32 &r_idx, uint32 len)
	{
		r_idx += len;
		r_idx %= m_mem->m_buf_size;
	}


	void offsetWriteIdx(uint32 &w_idx, uint32 len)
	{
		w_idx += len;
		w_idx %= m_mem->m_buf_size;
	}
private:
	uint32 next(const uint32 current) const
	{
		return (current + 1) % m_mem->m_buf_size;
	}


private:
	CirMsgQueMem * m_mem;
};



//ʵ�ú��񲻺�,������ѧϰ�ɱ��� ��˼���費��Ҫ��
namespace EasyRW
{

//��ֹ����ڴ�copy ��д. ��CircleBuf
//����ֵ�ο� ::recv
template<typename SOCKET, typename CircleBuf>
int recv(SOCKET &socket, CircleBuf &buf)
{
	typedef typename CircleBuf::ValueType ValueType;
	uint32 len = 0;
    
	ValueType *p_write = buf.getCanContinueWrite(len);;
    if (NULL == p_write)
    {
        return 0;
    }
    
	int ret = ::recv(socket, (uint8 *)p_write, len*sizeof(ValueType), 0);
	if (0 == ret || -1 == ret)
	{
		return ret;
	}
	buf.finishWillWrite(ret);
	return ret;
}

//����ֵ�ο� ::send,
template<typename SOCKET, typename CircleBuf>
int send(SOCKET &socket, CircleBuf &buf){
	typedef typename CircleBuf::ValueType ValueType;
	uint32 len = 0;
	const ValueType *p_read = buf.getCanContinueRead(len);
    if (NULL == p_read)
    {
        return 0;
    }
    
	int ret = ::send(socket, (const char *)p_read, len*sizeof(ValueType), 0);
	if (0 == ret || -1 == ret)
	{
		return ret;
	}
	buf.finishWillRead(ret);
	return ret;
}

//����ֵ�ο� Ϊbuf��ȡԪ�ص�����
template<typename CircleBuf>
int write(std::ostream &s, CircleBuf &buf)
{
	typedef typename CircleBuf::ValueType ValueType;
	uint32 len = 0;
	const ValueType *p_read = buf.getCanContinueRead(len);
    if (NULL == p_read)
    {
        return 0;
    }
    
	s.write((const char *)p_read, len*sizeof(ValueType));
	s.flush();
	buf.finishWillRead(len);
	return len;
}

}//namespace EasyRW

