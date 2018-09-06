/*
brief:
对象的内存池
特点：
快速分配释放
快速查找

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
		//:pid(0)不能初始化，构造前就初始化对了
	{
	}
	uint64 Pid() const{return pid;};

	//用户不要用这个接口，用了就是错的。
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

//id快速查找内存池
//id包含idx信息，random access element.
template <typename T, size_t MAX_NUM>
class PoolIdIdx
{
private:
	StructId::SubId m_id_seed;			
	T m_data[MAX_NUM];		//用数组，地址稳定，不重分配
	std::vector<T *> m_free;	//元素为指针， 指向 m_data 的元素地址
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

	//分配，构造对象
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
		if (idx >= MAX_NUM)//测试出错用,稳定后可以删除
		{
			printf("logic error!!");
		}
		pObj->SetPid(idx, sub_id);
		new ((void  *)pObj) T();
		return pObj;
	}

	//释放，析构对象
	//注意，传入野id,有低概率导致未定义行为
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
		if(0 == st_id.sub_id) //0肯定是野id,能避免不少错误
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


//实现基类，非接口基类
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

//可预分配， 从系统分配了内存，就不释放给系统，给用户复用。
//优点：快速分配释放内存
class MemoryPool
{
    typedef std::vector<char *> VecPoint;
public:
    //para:size_t one_obj_size, 对象sizeof 
    //para:size_t reserver_num, 每次预分配对象数量
    MemoryPool(size_t one_obj_size, size_t reserver_num);
    ~MemoryPool();
    void *AllocObj();
    void FreeObj(void *p);

private:
    bool reserve();

private:
    VecPoint m_free_addr;       //保存可以分配给用户的对象地址
    VecPoint m_sys_alloc_addr;  //保存系统分配内存地址
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
