#include "../utility/circleBuf.h"
#include <assert.h>


bool CirMsgQue::InitMemory(char *use_mem, uint32 len)
{
	if (len < sizeof(CirMsgQueMem) || nullptr == use_mem)
	{
		return false;
	}

	CirMsgQueMem * p = (CirMsgQueMem *)use_mem;
	p->m_buf_size = len - sizeof(CirMsgQueMem)+1;
	p->m_r_idx = 0;
	p->m_w_idx = 0;
	return true;
}

CirMsgQue::CirMsgQue()
:m_mem(NULL)
{
}

bool CirMsgQue::Init(char *use_mem, uint32 len)
{
	if (len < sizeof(CirMsgQueMem))
	{
		return false;
	}

	m_mem = (CirMsgQueMem *)use_mem;
	if (m_mem->m_buf_size != len - sizeof(CirMsgQueMem)+1)
	{
		return false; //未初始化 use_mem
	}

	return true;
}
//已写字节数量
uint32 CirMsgQue::len() const
{
	uint32 w_idx = m_mem->m_w_idx;
	uint32 r_idx = m_mem->m_r_idx;

	//读取一个副本计算副本的已写字节数，避免直接使用，计算过程被别的进程改动变量了。
	if (w_idx >= r_idx)
	{
		return w_idx - r_idx;
	}
	else
	{
		return m_mem->m_buf_size - r_idx + w_idx;
	}
}

uint32 CirMsgQue::freeLen() const
{
	//m_buf 数组长度， 实际能写最大长度少1. 目的用来区分满或者空. m_r_index==m_w_index 表示空, 可用内存一个字节为满
	return m_mem->m_buf_size -1 - len();
}

bool CirMsgQue::Write(const char* buf, size_t _len)
{
	//非法检查
	if (0 == _len || _len+sizeof(_len) > freeLen() || NULL == buf)
	{
		return false;
	}

	uint32 w_idx = m_mem->m_w_idx; //用w_idx临时计算，最后全部操作成功再修改  m_mem->m_w_idx
	uint32 r_idx = m_mem->m_r_idx;
	//先写头
	uint32 to_end_len = m_mem->m_buf_size - w_idx;
	if (r_idx > w_idx || sizeof(_len) <= to_end_len)
	{  //不分段copy的情况
		memcpy((void *)&m_mem->m_buf[w_idx], (const void *)&_len, sizeof(_len));
	}
	else
	{
		//case _len > to_end_len
		memcpy((void *)&m_mem->m_buf[w_idx], (const void *)&_len, to_end_len);
		//write to r-1
		memcpy((void *)&m_mem->m_buf[0], (const void *)((char *)&_len + to_end_len), sizeof(_len)-to_end_len);
	}
	offsetWriteIdx(w_idx, sizeof(_len));

	//同样方法写消息内容
	to_end_len = m_mem->m_buf_size - w_idx;
	if (r_idx > w_idx || _len <= to_end_len)
	{  //不分段copy的情况
		memcpy((void *)&m_mem->m_buf[w_idx], (const void *)buf, _len);
	}
	else
	{
		//case _len > to_end_len
		memcpy((void *)&m_mem->m_buf[w_idx], (const void *)buf, to_end_len);
		//write to r-1
		memcpy((void *)&m_mem->m_buf[0], (const void *)(buf + to_end_len), _len - to_end_len);
	}
	offsetWriteIdx(w_idx, _len);
	m_mem->m_w_idx = w_idx;
	//printf("write len=%d\n", _len);
	return true;
}

uint32 CirMsgQue::Read(char *buf, uint32 _len)
{
	if (NULL == buf || len() < sizeof(uint32))
	{
		return 0;
	}

	uint32 r_idx = m_mem->m_r_idx; //用w_idx临时计算，最后全部操作成功再修改  m_mem->m_r_idx
	uint32 w_idx = m_mem->m_w_idx;
	//先读头
	uint32 msg_len = 0;
	uint32 to_end_len = m_mem->m_buf_size - r_idx;
	if (w_idx >= r_idx || sizeof(uint32) <= to_end_len)
	{ //不分段copy的情况
		memcpy((void *)&msg_len, (const void *)&m_mem->m_buf[r_idx], sizeof(uint32));
	}
	else
	{
		//case _len > to_end_len
		//read to array end 
		memcpy((void *)&msg_len, (const void *)&m_mem->m_buf[r_idx], to_end_len);
		//read to r-1
		memcpy((void *)((char *)&msg_len + to_end_len), (const void *)&m_mem->m_buf[0], (sizeof(uint32)-to_end_len));
	}

	if (0 == _len || _len < msg_len)
	{
		return 0;
	}
	offsetRead(r_idx, sizeof(uint32));

	//printf("msg_len=%d\n", msg_len);
	to_end_len = m_mem->m_buf_size - r_idx;
	//printf("to_end_len=%d\n", to_end_len);
	if (w_idx >= r_idx || msg_len <= to_end_len)
	{ //不分段copy的情况
		memcpy((void *)buf, (const void *)&m_mem->m_buf[r_idx], msg_len);
	}
	else
	{
		//case _len > to_end_len
		//read to array end 
		memcpy((void *)buf, (const void *)&m_mem->m_buf[r_idx], to_end_len);
		//read to r-1
		memcpy((void *)(buf + to_end_len), (const void *)&m_mem->m_buf[0], (msg_len - to_end_len));
	}
	offsetRead(r_idx, msg_len);
	m_mem->m_r_idx = r_idx;
	return msg_len;
}


