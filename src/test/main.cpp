// testPrj.cpp : 定义控制台应用程序的入口点。
//

#include "MoreThread/Thread.h"



using namespace std;
using namespace wyl;

void testUtility();
 void testStaticReg();
void testMemoryStream();
void testTimer();
void testBagAndItem();;
void testRandom();
void test_client();
void testPrj2();
void test_obj_stream();
void testRankFun();
void testQueueEvent();
void testSimpleBag();
void testUniquId();
void testRegion();
void test_wyl_time();
void test_game_cheat();
void testFriend();
void test_mult_key_map();
void testMission();
void test_regex();
void testRank();
void testMoreThread();
void test_misc();
void testStringTool();
void testLogFile();
void testReadCfg();
void testObjPool();
void testEnumName();
void testGameTask();
void TestRank();
void test_single_progresss();
void test_delay_handler();
void testDelayPara();

namespace
{
	DefaultLog my_log("log.txt");
}
 
void MyUnitTestPrintf(bool is_error, const char * file, int line, const char *fun, const char * pattern, va_list vp)
{
	if (is_error)
	{
		my_log.Printf(LL_ERROR, file, line, fun, pattern, vp);
	} 
	else
	{
		my_log.Printf(LL_DEBUG, file, line, fun, pattern, vp);
	}
}



int main(int argc, char* argv[])
{
	L_DEBUG("start");
	UnitTestMgr::Obj().Start(MyUnitTestPrintf);
	testLogFile();
	test_wyl_time();
	//return 0;
}

GlobalReg<string> gdd("2");
