// testPrj.cpp : 定义控制台应用程序的入口点。
//

#include "MoreThread/Thread.h"



using namespace std;
using namespace wyl;
//using namespace boost;
//using namespace stdext;;


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



int main(int argc, char* argv[])
{
	DebugLog::GetDefaultLog().setStdOut(true);
	LOG_DEBUG("\nstart");

	//testRegion();
	//
	//TestRank();
	//testGameTask();
	//testStaticReg();
	//test_delay_handler();
   //testRank();
  // testUtility();
   // testStringTool();
    //testLogFile();
    //testReadCfg();
    test_wyl_time();
   //testTimer();
  //  testStaticReg();
	//testRandom();
	//testMemoryStream();
//	testUniquId();
	//testMoreThread();
	//test_single_progresss();
	while(1);
	return 0;
}

GlobalReg<string> gdd("2");
