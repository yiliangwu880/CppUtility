/*
brief:
������ڴ��
�ص㣺
���ٷ����ͷ�
���ٲ���

use example:
Class MyObj : public PoolIdIdxMgr<MyObj, 10>
{
...
};

MyObj *p = MyObj::Create();
MyObj::Free(p);
*/
#pragma once
#include "typedef.h"
#include "cnt_typedef.h"
#include <utility>
#include <memory.h>


struct StructId 
{
	typedef uint32 Idx;
	typedef uint32 SubId;
	Idx		idx;
	SubId	sub_id;
};

class PoolIdIdxObj
{
public:
	PoolIdIdxObj()
		//:pid(0)���ܳ�ʼ��������ǰ�ͳ�ʼ������
	{
	}
	uint64 Pid() const{return pid;};

	//�û���Ҫ������ӿڣ����˾��Ǵ�ġ�
	void SetPid(StructId::Idx idx, StructId::SubId sub_id)
	{
		st_id.idx	 = idx;
		st_id.sub_id = sub_id;
	}
	uint32 GetSubId() const {return st_id.sub_id;}
	uint32 GetIdx() const {return st_id.idx;}
private:
	union
	{
		uint64 pid;
		StructId st_id;
	};
};

//id���ٲ����ڴ��
//id����idx��Ϣ��random access element.
template <typename T, size_t MAX_NUM>
class PoolIdIdx
{
private:
	StructId::SubId m_id_seed;			
	T m_data[MAX_NUM];		//�����飬��ַ�ȶ������ط���
	std::vector<T *> m_free;	//Ԫ��Ϊָ�룬 ָ�� m_data ��Ԫ�ص�ַ
private:
	StructId::SubId CreateSubId()
	{
		++m_id_seed;
		if (0 == m_id_seed)
		{
			++m_id_seed;
		}
		return m_id_seed;
	}

public:
	PoolIdIdx()
		:m_id_seed(0)
	{
		memset(m_data, 0, sizeof(m_data));
		m_free.reserve(MAX_NUM);
		for (size_t i = MAX_NUM-1; i < MAX_NUM; --i)
		{
			m_free.push_back(&m_data[i]);
		}
	}

	//���䣬�������
	inline T * Create()
	{
		if (m_free.empty())
		{
			return NULL;
		}
		T *pObj = m_free.back();
		m_free.pop_back();

		StructId::Idx idx = pObj - m_data;
		StructId::SubId sub_id = CreateSubId();
		if (idx >= MAX_NUM)//���Գ�����,�ȶ������ɾ��
		{
			printf("logic error!!");
		}
		pObj->SetPid(idx, sub_id);
		new ((void  *)pObj) T();
		return pObj;
	}

	//�ͷţ���������
	//ע�⣬����Ұid,�е͸��ʵ���δ������Ϊ
	inline bool Free(const StructId &st_id)
	{
		if (st_id.idx >= MAX_NUM)
		{
			return false;
		}
		if(m_data[st_id.idx].GetSubId() != st_id.sub_id)
		{
			return false;
		}
		if(0 == st_id.sub_id) //0�϶���Ұid,�ܱ��ⲻ�ٴ���
		{
			return false;
		}
		T *pObj = &m_data[st_id.idx];
		pObj->SetPid(-1, 0);
		pObj->~T();
		m_free.push_back(pObj);
		return true;
	}

	inline T *GetObj(const uint64 pid)
	{
		const StructId &st_id = reinterpret_cast<const StructId&>(pid);
		if (st_id.idx >= MAX_NUM)
		{
			return NULL;
		}
		if(m_data[st_id.idx].GetSubId() != st_id.sub_id)
		{
			return NULL;
		}
		return &m_data[st_id.idx];
	}


	uint32 GetUseNum()
	{
		return MAX_NUM-m_free.size();
	}

	template<class Function>
	void ForEachObj(Function _func)
	{
		for (uint32 i=0; i<MAX_NUM; ++i)
		{
			if(0 != m_data[i].sub_id)
			{
				_func(m_data[i]);
			}
		}
	}

};


//ʵ�ֻ��࣬�ǽӿڻ���
template <typename T, size_t MAX_NUM>
class PoolIdIdxMgr : public PoolIdIdxObj
{
private:
	typedef PoolIdIdx<T, MAX_NUM> Pool;
	static Pool &pool(){
		static Pool s_pool;
		return s_pool;
	};
public:
	static T *Create()
	{
		return pool().Create();
	}

	static bool Free(uint64 pid)
	{
		StructId &id = reinterpret_cast<StructId&>(pid);
		return pool().Free(id);
	}

	static T *GetObj(uint64 pid)
	{
		return  pool().GetObj(pid);
	}

	static uint32 GetUseNum()
	{
		return pool().GetUseNum();
	}
};

//��Ԥ���䣬 ��ϵͳ�������ڴ棬�Ͳ��ͷŸ�ϵͳ�����û����á�
//�ŵ㣺���ٷ����ͷ��ڴ�
class MemoryPool
{
    typedef std::vector<char *> VecPoint;
public:
    //para:size_t one_obj_size, ����sizeof 
    //para:size_t reserver_num, ÿ��Ԥ�����������
    MemoryPool(size_t one_obj_size, size_t reserver_num);
    ~MemoryPool();
    void *AllocObj();
    void FreeObj(void *p);

private:
    bool reserve();

private:
    VecPoint m_free_addr;       //������Է�����û��Ķ����ַ
    VecPoint m_sys_alloc_addr;  //����ϵͳ�����ڴ��ַ
    const size_t m_one_obj_size;
    const size_t m_reserver_num;
};

template<class T>
class ObjPool
{
public:
    explicit ObjPool(size_t reserver_num)
        :m_pool(sizeof(T), reserver_num)
    {
    }

    T *Create()
    {
        T *p = (T *)m_pool.AllocObj();
        if (NULL == p)
        {
            return NULL;
        }

        new(p) T;
        return p;
    }

    void Destroy(T *p)
    {
        if (NULL == p)
        {
            return;
        }
        p->~T();
        m_pool.FreeObj(p);
    }

private:
    MemoryPool m_pool;
};
