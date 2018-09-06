/*
brief:线程同步相关

*/

#pragma once

class Token;

//参考ace 写模板，支持不同类型院子操作
namespace ZThread {

  /**
   * @class AtomicCount
   * @author Eric Crahen <http://www.code-foo.com>
   * @date <2003-07-16T09:41:55-0400> 
   * @version 2.3.0
   *
   * This class provides an interface to a small integer whose value can be
   * incremented or decremented atomically. It's designed to be as simple and
   * lightweight as possible so that it can be used cheaply to create reference
   * counts.
   */
  class ZTHREAD_API AtomicCount : public NonCopyable {
  
    void* _value;
  
  public:
  
    //! Create a new AtomicCount, initialized to a value of 1
    AtomicCount();

    //! Destroy a new AtomicCount
    ~AtomicCount();
  
    //! Postfix decrement and return the current value
    size_t operator--(int);
  
    //! Postfix increment and return the current value
    size_t operator++(int);  

    //! Prefix decrement and return the current value
    size_t operator--();
  
    //! Prefix increment and return the current value
    size_t operator++();  


  }; /* AtomicCount */

  
} // namespace ZThread

template <class LOCK = Thread_Mutex,
class TYPE = u_long>
class Atomic_Op
{
	// Same as before
};
// ...

//参考
class Resource_Manager
{
public:
	Resource_Manager (u_int initial_resources)
		: resource_add_ (this->lock_),
		resources_ (initial_resources) {}
	int acquire_resources (u_int amount_wanted)
	{
		this->lock_.acquire ();
		while (this->resources_ < amount_wanted)
		{
			this->waiting_++;
			// Block until resources are released.
			this->resource_add_.wait ();
		}
		this->resources_ -= amount_wanted;
		this->lock_.release ();
	}
	int release_resources (u_int amount_released)
	{
		this->lock_.acquire ();
		this->resources_ += amount_released;
		if (this->waiting_ == 1)
		{
			this->waiting_ = 0;
			17
				this->resource_add_.signal ()
		}
		else if (this->waiting_ > 1)
		{
			this->waiting_ = 0;
			this->resource_add_.broadcast ();
		}
		this->lock_.release ();
	}
	// ...
private:
	ACE_Mutex lock_;
	ACE_Condition<ACE_Mutex> resource_add_;
	u_int resources_;
	u_int waiting_;
	// ...
};


template <class MUTEX>
class ACE_Guard
{
public:
	ACE_Guard (MUTEX &m): lock_ (m)
	{
		this->lock_.acquire ();
	}
	_Guard (void)
	{
		this->lock_.release ();
	}
private:
	MUTEX &lock_;
}


