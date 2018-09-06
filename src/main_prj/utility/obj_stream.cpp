
#include "obj_stream.h"

using namespace std;


	StreamObj * StreamObj::newAndWrite( const Memory &in )
	{
		string name;
		in >> name;
		StrMapCreate::const_iterator it = StrMapCreate::obj().find(name);
		if (it == StrMapCreate::obj().end())
		{
			return NULL;
		}
		CreateFun p_fun = it->second;
		StreamObj *p_obj = (*p_fun)(); 
		if (NULL == p_obj)
		{
			return NULL;
		}
		p_obj->doWrite(in);
		return p_obj;
	}

	bool StreamObj::read( Memory &out ) const
	{
		string s= typeid(*this).name();
		out << s;
		return doRead(out);
	}



//file end