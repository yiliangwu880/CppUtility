/*
brief: serialize and unserialize memory
����ɹ��ܣ�
1����stl���� д�ӿڣ����棬�Ͷ�ȡ ����Ԫ������
2������ת����bool,�����ж��Ƿ��������д if(in >> data)
3)Memory ��ӳ�Ա���ο����룺
/// ����endl��ends
inline basic_ostrbuf& operator<<(basic_ostrbuf& (__cdecl * fn)(basic_ostrbuf&))
{
return ((*fn)(*this));
}

/// ������һ������
inline basic_ostrbuf& operator<<(const basic_ostrbuf& other)
{
return push(other.c_str(), other.length());
}

*/

#pragma once
#include "typedef.h"
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>


//attention: ������ʼ��������ڴ棬�ڶ�������ڼ䲻���޸ġ�
//�����д�����ö���������Ķ�д��������Ч
class Memory
{
private:
	Memory(const Memory&); 
	Memory & operator= (const Memory &);
public:
	Memory(uint8 *pBuf, uint32 len);
	Memory(const uint8 *pBuf, uint32 len);

	//serialize
	bool write(const uint8 *outt, uint32 len);

	//unserialize
	bool read(uint8 *out, uint32 len) const;

	uint32 offset() const;
	//void offset( uint32 _offset) ; //�ı�offset,���ã�
	//ָ��д��λ�÷�ʽд�������Ҫ���Ժ��Ǽ���
	//bool write(const uint8 *outt, uint32 len, uint32 offset); 

	bool error() const;
	void setError() const;
	operator bool() const
	{
		return !error();
	}

	template<typename T>
	inline Memory & operator<< ( const T &t)
	{
		if (m_error)
		{
			return *this;
		}
		if (m_offset+sizeof(t) > m_maxLen)
		{
			m_error = true;
			return *this;
		}
		*(T *)(m_pBuf+m_offset)=t;
		m_offset+=sizeof(t);

		return *this;
	}

	template<typename T>
	inline const Memory & operator>> (T &t) const
	{
		if (m_error)
		{
			return *this;
		}
		if (m_offset+sizeof(t) > m_maxLen)
		{
			m_error = true;
			return *this;
		}
		t = *(const T *)(m_pBuf+m_offset);
		m_offset+=sizeof(t);
		return *this;
	}


	inline Memory & operator<< (const std::string &t)
	{
		if (m_error)
		{
			return *this;
		}
		uint32 len = t.length();
		*this<<len;
		write((const uint8 *)t.c_str(), len);
		return *this;
	}


	inline const Memory & operator>> (std::string &t) const
	{
		uint32 len;
		*this>>len;

		if (m_error)
		{
			return *this;
		}
		if (m_offset+len > m_maxLen)
		{
			m_error = true;
			return *this;
		}
		t.assign((const char *)(m_pBuf+m_offset),len);
		m_offset+=len;
		return *this;
	}
	
	//������д
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline Memory & operator<< (const std::vector<T> &c)
	{
		writeCtn(c);
		return *this;
	}
	template<typename T>
	inline Memory & operator<< (const std::list<T> &c)
	{
		writeCtn(c);
		return *this;
	}

	template<typename T>
	inline Memory & operator<< (const std::deque<T> &c)
	{
		writeCtn(c);
		return *this;
	}

	template<typename Key, typename Value>
	inline Memory & operator<< (const std::map<Key, Value> &c)
	{
		writeMap(c);
		return *this;
	}


	template<typename T>
	inline Memory & operator<< (const std::set<T> &c)
	{
		writeCtn(c);
		return *this;
	}

	template<typename T>
	inline const Memory & operator>> ( std::vector<T> &c) const
	{
		readLineCtn(c);
		return *this;
	}

	template<typename T>
	inline const Memory & operator>> ( std::list<T> &c) const
	{
		readLineCtn(c);
		return *this;
	}

	template<typename T>
	inline const Memory & operator>> ( std::deque<T> &c) const
	{
		readLineCtn(c);
		return *this;
	}

	template<typename Key, typename Value>
	inline const Memory & operator>> ( std::map<Key, Value> &c) const
	{
		readMap(c);
		return *this;
	}

	template<typename T>
	inline const Memory & operator>> ( std::set<T> &c) const
	{
		readSet(c);
		return *this;
	}
//////////////////////////////////////////////////////////////////////////


private:

	template<typename T>
	inline void writeCtn (const T &c)
	{
		uint32 len = c.size();
		*this << len;
		for(typename T::const_iterator it=c.begin(); it!=c.end(); ++it)
		{
			*this << *it;
		}
	}

	template<typename T>
	inline void writeMap (const T &c)
	{
		uint32 len = c.size();
		*this << len;
		for(typename T::const_iterator it=c.begin(); it!=c.end(); ++it)
		{
			*this << it->first;
			*this << it->second;
		}
	}

	//�����߼��ṹ����vector list deque	��ȡ
	template<typename T>
		inline void readLineCtn (T &c) const
	{
		uint32 len = 0;
		*this >> len;
		for(uint32 i=0; i<len; ++i)
		{
			typename T::value_type data;
			*this >> data;
			c.push_back(data);
		}
	}

	template<typename T>
	inline void readSet (T &c) const
	{
		uint32 len = 0;
		*this >> len;
		for(uint32 i=0; i<len; ++i)
		{
			typename T::value_type data;
			*this >> data;
			bool r = c.insert(data).second;
			if (!r)
			{
				setError();
				return;
			}
		}
	}

	template<typename T>
	inline void readMap (T &c) const
	{
		uint32 len = 0;
		*this >> len;
		for(uint32 i=0; i<len; ++i)
		{
			typename T::key_type key;
			typename T::mapped_type map_type;
			*this >> key;
			*this >> map_type;
			bool r = c.insert(std::make_pair(key, map_type)).second;
			if (!r)
			{
				setError();
				return;
			}
		}
	}


private:
	uint8 *m_pBuf;
	mutable uint32 m_offset; // current offset for have read or write
	uint32 m_maxLen;
	mutable bool m_error;
};





