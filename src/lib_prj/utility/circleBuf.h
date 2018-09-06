/*
author: YiliangWu
*/

#pragma once

#include "typedef.h"
#include <iostream>
#include "string.h"
#include <sys/socket.h>

//无锁队列，支持一个线程写，另一个线程读。
//para:typename T, 元素类型
//para:uint32 MAX_SIZE, 存放最大元素个数
template<typename T, uint32 MAX_SIZE>
class CircleBuf
{
	const static uint32 ARRAY_LEN = MAX_SIZE+1; //必须留一个空位置不用，用来区分满或者空. m_r_index==m_w_index 表示空, 可用内存剩一个元素空间为满

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

    //满的情况，没修改
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


    //拥有元素数量
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

    //free元素数量
    uint32 freeLen() const
    {
		return MAX_SIZE - len();
    }
	
	//缓存任意内存，失败不做任何修改
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
		{  //不分段copy的情况
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

    //成功就是读满，失败没变化。
    //para: uint32 _len 为 *t指向的数组长度
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
        { //不分段copy的情况
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

	//获取连续的内存，作为已经写入的缓存。然后函数调用完成后再写入具体内容
    //注意，该接口暴露成员地址给用户，需要用户确保代码正确。否则结果未定义。
    //改接口目的是给recv等函数，效率直接copy内存，不用中间多copy一份
	/*
		使用例子：
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
    //失败返回NULL,成功返回 _len 长度的有效内存地址
	ValueType *getCanContinueWrite(uint32 &_len)
	{
        _len=enableContinuousWriteLen();
		if (0 == _len)
		{
			return NULL;
		}
		return &m_buf[m_w_idx];
	}

    //获取连续的读出的缓存内存地址。读取完成后，再调用完成读出内容。
    //注意，该接口暴露成员地址给用户，需要用户确保代码正确。否则结果未定义。
    //改接口目的是给recv等函数，效率直接copy内存，不用中间多copy一份
	/*
		使用例子：
		uint32 len = 0;
		T *p=obj.getWillRead(len);
        if(p != NULL)
        {
		    memcpy(xx,xx,xx);
		    obj.finishWillRead(len);
        }
	*/
    //失败返回NULL,成功返回 _len 长度的有效内存地址
    //uint32 &_len, 返回可以连续写的长度
	ValueType *getCanContinueRead(uint32 &_len)
	{
        _len = enableContinuousReadLen();
		if (0 == _len)
		{
			return NULL;
		}
		return &m_buf[m_r_idx];
	}

    //uint32 len, 元素数量
    bool finishWillWrite(uint32 len)
    {
        if (len>enableContinuousWriteLen())
        {
            return false;
        }
       offsetWriteIdx(len);
       return true;
    }


    //预先读取，不改变对象
    //成功就是读满，失败没变化。
    //para: uint32 _len 为 *t指向的数组长度
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
        { //不分段copy的情况
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
		uint32 w_idx = m_w_idx; //取瞬间副本计算

		if (w_idx >= r_idx)
        { //不分段copy的情况
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
        {  //不分段copy的情况
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
	uint32 m_r_idx; //指向下一个将要读的索引
	uint32 m_w_idx; //指向下一个将要写的索引
	T m_buf[ARRAY_LEN];
};


struct CirMsgQueMem
{
	uint32 m_r_idx;		//指向下一个将要读的索引
	uint32 m_w_idx;		//指向下一个将要写的索引
	uint32 m_buf_size;	//m_buf 数组长度， 实际能写最大长度少1. 目的用来区分满或者空. m_r_index==m_w_index 表示空, 可用内存一个字节为满
	char m_buf[1];		//可变长度数组
};

//@brief 循环队列读写，支持一个线程无锁写，另一个线程无锁读
//	addr. circle message queue write
class CirMsgQue
{
public:

	//@para use_mem 要操作的内存（可以是共享内存）
	//@para len use_mem有效字节数
	//注意： len长度必须>=sizeof(CirMsgQueMem) 
	static bool InitMemory(char *use_mem, uint32 len);

	CirMsgQue();

	//@para use_mem 要操作的内存（可以是共享内存）
	//@para len use_mem有效字节数
	//注意： use_mem 必须用 InitMemory 初始化，不然失败
	bool Init(char *use_mem, uint32 len);

	bool Write(const char* buf, size_t len);
	
	//@para buf 数据写入地址
	//@para len buf有效空间字节数
	//@return 返回读取长度，0表示失败
	uint32 Read(char* buf, uint32 len);

	//返回 Write 函数能写的长度 
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

	//已写字节数量
	uint32 len() const;

	//未写字节数量
	//Write函数实际需求字节数=Write请求长度+sizeof(uint32))
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



//实用好像不好,增加了学习成本， 待思考需不需要。
namespace EasyRW
{

//防止多次内存copy 读写. 用CircleBuf
//返回值参考 ::recv
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

//返回值参考 ::send,
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

//返回值参考 为buf读取元素的数量
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

