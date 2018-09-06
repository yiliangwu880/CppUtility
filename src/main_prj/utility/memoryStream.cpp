
#include "memoryStream.h"
#include <memory.h>

	Memory::Memory( uint8 *pBuf, uint32 len )
		:m_pBuf(pBuf)
		,m_offset(0)
		,m_maxLen(len)
		,m_error(false)
	{
	}

	Memory::Memory( const uint8 *pBuf, uint32 len ) 
		:m_pBuf((uint8 *)pBuf)
		,m_offset(0)
		,m_maxLen(len)
		,m_error(false)
	{

	}

	bool Memory::write( const uint8 *out, uint32 len )
	{
		if (m_error)
		{
			return false;
		}
		if (m_offset+len > m_maxLen)
		{
			m_error = true;
			return false;
		}
		memcpy((void *)(m_pBuf+m_offset), (const void *)out, len);
		m_offset+=len;
		return true;
	}

	bool Memory::read( uint8 *out, uint32 len ) const
	{
		if (m_error)
		{
			return false;
		}
		if (m_offset+len > m_maxLen)
		{
			m_error = true;
			return false;
		}
		memcpy((void *)out, (const void *)(m_pBuf+m_offset), len);
		m_offset+=len;
		return true;
	}

	bool Memory::error() const
	{
		return m_error;
	}

	void Memory::setError() const
	{
		m_error = true;
	}

	uint32 Memory::offset() const
	{
		return m_offset;
	}

	//void MemorySerialize::offset( uint32 _offset ) 
	//{
	//	m_offset=_offset;
	//	if (m_offset>m_maxLen)
	//	{
	//		m_error = true;
	//	}
	//}




