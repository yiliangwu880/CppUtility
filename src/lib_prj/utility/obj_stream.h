/*
brief: 动态对象的流化 功能
*/

#pragma once
#include "typedef.h"
#include <string>
#include "memoryStream.h"
#include "staticReg.h"
#include <typeinfo>


/*
use example:
//h file
//////////////////////////////////////////////////////////////////////////
	Class DeriveClass : public StreamObj
	{
	public:
	private:
		virtual bool doRead(MemorySerialize &out) const;
		virtual bool doWrite(const MemorySerialize &in);
	}

//cpp file
//////////////////////////////////////////////////////////////////////////
	NEW_STREAM_OBJ_DEFINE(DeriveClass);
	
	//read obj
	{
		MemorySerialize out;
		...
		p_obj->read(out);
		delete p_obj;
	}	

	//write obj
	{
		MemorySerialize in;
		...
		StreamObj *p_obj = StreamObj::new_and_write(in);

	}
*/


#define NEW_STREAM_OBJ_DEFINE(DeriveClass)\
	namespace{\
		StreamObj *create##DeriveClass()\
		{\
			return new DeriveClass;\
		}\
	}\
	MAP_REG_NAME_DEFINE(StrMapCreate, g_##DeriveClass, typeid(DeriveClass).name(), create##DeriveClass);

	class StreamObj
	{
	public:
		typedef StreamObj *(*CreateFun)(); 

	public:
		virtual ~StreamObj(){};
		//new and write obj by stream memory.
		static StreamObj *newAndWrite(const Memory &in); 
		//read stream obj
		bool read(Memory &out) const;  
	protected:
		virtual bool doRead(Memory &out) const = 0;  //read stream obj
		virtual bool doWrite(const Memory &in) = 0;  //write stream obj

	};


	REG_MAP_NAME_DECLARE(StrMapCreate, std::string, StreamObj::CreateFun)  
 

