
/*
不能用递归互斥体。
*/
#pragma once



class 
{
public:
	void wait()
	{
		mutex.lock
		while(!condition)
		{
			wait(cont, mutex);
		}
		mutex.unlock
	}
	void changeC()
	{
		mutex.lock
			change condition
			signal(cont, mutex);

		mutex.unlock
	}	

private:
	virtual CheckCondition();
	virtual 

};