
#include "obj_pool.h"
#include "stdio.h"
#include "misc.h"

using namespace std;

MemoryPool::MemoryPool( size_t one_obj_size, size_t reserver_num )
    :m_one_obj_size(one_obj_size)
    ,m_reserver_num(reserver_num)
{
    reserve();
}

MemoryPool::~MemoryPool()
{
    FOR_IT(VecPoint, m_sys_alloc_addr)
    {
        char *p = *it;
        delete[] p;
    }
}

bool MemoryPool::reserve()
{
    if (0 == m_one_obj_size || 0 == m_reserver_num )
    {
        printf("error, new memory fail! num=%lud", m_one_obj_size*m_reserver_num);
        return false;
    }
    char *p = new char[m_one_obj_size*m_reserver_num];
    if (NULL == p)
    {
        printf("error, new memory fail! num=%lud", m_one_obj_size*m_reserver_num);
        return false;
    }

    m_sys_alloc_addr.push_back(p);
    for (size_t i=0; i<m_reserver_num; ++i)
    {
        m_free_addr.push_back(p);
        p+=m_one_obj_size;
    }
    return true;
}

void * MemoryPool::AllocObj()
{
    if (m_free_addr.empty())
    {
        if (!reserve())
        {
            return NULL;
        }
    }

    char *p = m_free_addr.back();
    m_free_addr.pop_back();
    return p;
}

void MemoryPool::FreeObj( void *p )
{
    m_free_addr.push_back((char *)p);
}
