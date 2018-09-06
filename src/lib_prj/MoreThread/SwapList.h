/**H**************************************************************************
 File            : SwapList.h
 Subsystem       : 
 Function Name(s): CSwapList
 Author          : YiQiu
 Date            : 2012-3-26  
 Time            : 18:11
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen Team Oleaster.
***************************************************************************H*/
#ifndef _SWAPLIST_H_2012_3
#define _SWAPLIST_H_2012_3
#include <list>

namespace OCI
{

/**
@name : 翻转队列
@brief: 在多线程生产者-消费者模式中生产者线程向队列添加对象，消费者线程从队列取走对象
        因为涉及到多线程访问，这个队列必须在添加/取走对象时加锁。这个模版实现这种多线程
		安全的队列，另外使用两个队列翻转的技术，可以极大的减少锁互斥的几率
*/

template<typename TObj, typename TLock>
class CSwapListBase
{
protected:
	void Swap()
	{
		m_frontLock.Lock();
        {
            ProductList* t = m_frontList;
            m_frontList = m_backList;
            m_backList = t;
            //std::swap(m_frontList, m_backList);
        }
        m_frontLock.UnLock();
	}

protected:
	CSwapListBase()
        : m_frontList(new ProductList)
        , m_backList(new ProductList){}
	virtual ~CSwapListBase(){delete m_frontList; delete m_backList;}

protected:
	typedef std::list<TObj> ProductList;
	ProductList *m_frontList, *m_backList;
	TLock m_frontLock;

};

/**
*@note 仅仅一个线程Get，多个线程Add的交换队列
*/
template<typename TObj, typename TLock>
class CSwapList1 :
    public CSwapListBase<TObj, TLock>
{
public:
    void Add(TObj& obj)
    {
        m_frontLock.Lock();
        {
            m_frontList->push_back(obj);
        }
        m_frontLock.UnLock();
    }

    //2012-07-20
    //add by frank
    //add beg
    void AddFront(TObj& obj)
    {
        m_frontLock.Lock();
        {
            m_frontList->push_front(obj);
        }
        m_frontLock.UnLock();
    }
    //add end

	bool Get(TObj& obj)
	{
		if ( m_backList->empty() )
		{
			Swap();
            if ( m_backList->empty() )
            {
            	return false;
            }
		}

	    obj = m_backList->front();
	    m_backList->pop_front();
		return true;
	}

    bool Empty()
    {
        bool bEmpty = false;
        m_frontLock.Lock();

        bEmpty = (m_backList->empty() && m_frontList->empty());

        m_frontLock.UnLock();

        return bEmpty;
    }
};

/**
*@note 多个线程Get，多个线程Add的交换队列
*/
template<typename TObj, typename TLock>
class CSwapList2 :
    public CSwapListBase<TObj, TLock>
{
public:
    CSwapList2():m_nCount(0){}

public:
    void Add(TObj& obj)
    {
        m_frontLock.Lock();
        {
            ++m_nCount;
            m_frontList->push_back(obj);
        }
        m_frontLock.UnLock();
    }

	bool Get(TObj& obj)
	{
		if ( m_nCount == 0 )
		{
			return false;
		}

        m_backLock.Lock();
		if ( m_backList->empty() )
		{
			Swap();
            if ( m_backList->empty() )
            {
                m_backLock.UnLock();
            	return false;
            }
		}
        
        --m_nCount;
	    obj = m_backList->front();
	    m_backList->pop_front();
        m_backLock.UnLock();
		return true;
	}

protected:
    volatile long m_nCount;
    TLock m_backLock;
};

/**
*@note 仅仅一个线程Get，多个线程Add的交换队列,并且提供取空back_list再swap的接口
*/
template<typename TObj, typename TLock>
class CSwapList3 :
    public CSwapList1<TObj, TLock>
{
public:
    bool GetBackList(TObj& obj)
    {
        if ( m_backList->empty() )
            return false;

        obj = m_backList->front();
        m_backList->pop_front();
        return true;
    }

    void SwapBackList()
    { Swap(); }
};

}
#endif  // _SWAPLIST_H_2012_3


 // End of file



