
#pragma once


class HeapObj
{
private:
	virtual ~HeapObj(){};
	void destroy() {delete this;}
};