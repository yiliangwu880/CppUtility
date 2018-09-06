/*
brief:
*/

#pragma once

class Reactor
{
public:

	void loop();
	void post_event(); //发事件，使在下一次事件分派中通知事件处理器处理
private:


};