#pragma once

#include <assert.h>
#include "noncopyable.h"

/************************************************************************/
/* 
Example:
class concreterClass : public SingletonBase<concreterClass>
{
private:
concreterClass(){};

};
*/
/************************************************************************/


template <typename T, bool INIT = true>
class SingletonBase : private Noncopyable
{
};

template <typename T>
class SingletonBase<T,true> : private Noncopyable
{
protected:
	SingletonBase() {}
	~SingletonBase() {}

public:
	static T& instance()
    {
        return obj();
	}
	static T& obj()
	{
		static T t;
		return t;
	}
};

