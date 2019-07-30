#pragma once

#include <assert.h>
#include "noncopyable.h"

/************************************************************************/
/* 
Example:
class concreterClass : public Singleton<concreterClass>
{
private:
concreterClass(){};

};
*/
/************************************************************************/


template <typename T, bool INIT = true>
class Singleton : private Noncopyable
{
};

template <typename T>
class Singleton<T,true> : private Noncopyable
{
protected:
	Singleton() {}
	~Singleton() {}

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

