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
@name : ��ת����
@brief: �ڶ��߳�������-������ģʽ���������߳��������Ӷ����������̴߳Ӷ���ȡ�߶���
        ��Ϊ�漰�����̷߳��ʣ�������б��������/ȡ�߶���ʱ���������ģ��ʵ�����ֶ��߳�
		��ȫ�Ķ��У�����ʹ���������з�ת�ļ��������Լ���ļ���������ļ���
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
*@note ����һ���߳�Get������߳�Add�Ľ�������
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
*@note ����߳�Get������߳�Add�Ľ�������
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
*@note ����һ���߳�Get������߳�Add�Ľ�������,�����ṩȡ��back_list��swap�Ľӿ�
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



