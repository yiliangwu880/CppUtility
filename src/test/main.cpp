// testPrj.cpp : 定义控制台应用程序的入口点。
//

#include "MoreThread/Thread.h"
#include "unit_test.h"
 

using namespace std;

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
void TestRank();
void test_single_progresss();
void test_delay_handler();
void testDelayPara();




int main(int argc, char* argv[])
{
	L_DEBUG("start");
	//UnitTestMgr::Ins().Start();
	UnitTestMgr::Ins().Run("testGameTask");
	//testLogFile();
	//return 0;
}

