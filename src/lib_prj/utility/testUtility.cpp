

#include "../utility/circleBuf.h"
#include "../MoreThread/Thread.h"
#include <vector>
#include <assert.h>
#include "misc.h"
#include "../Random/random.h"
#include <unistd.h>

using namespace std;

void testUtility();

namespace
{

	void testCircleBuf()
	{
		{
			CircleBuf<uint32, 3> buf;

			assert(buf.empty());
			assert(!buf.full());
			assert(0 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			buf.push(1);
			assert(!buf.empty());
			assert(!buf.full());
			//int i = buf.emptyLen();
			assert(1 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			uint32 r;
			buf.pop(r);
			assert(1==r);
			assert(0 == buf.len() && 3 == (buf.len()+buf.freeLen()));

			buf.push(1);
			buf.push(2);
			buf.push(3);
			assert(!buf.empty());
			assert(buf.full());
			assert(3 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			buf.pop(r);
			assert(1==r);
			assert(!buf.full());
			assert(2 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			buf.push(4);
			assert(!buf.empty());
			assert(buf.full());
			assert(3 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			buf.pop(r);
			assert(2==r);
			buf.pop(r);
			assert(3==r);
			buf.pop(r);
			assert(4==r);
			assert(false == buf.pop(r));
			assert(0==r);
			assert(buf.empty());
			assert(!buf.full());
			assert(0 == buf.len() && 3 == (buf.len()+buf.freeLen()));
		}
		{
			CircleBuf<uint32, 3> buf;
			uint32 ar[]={1,2,3,4};
			uint32 out[5];
			bool ret;
			assert(buf.empty());
			assert(!buf.full());
			assert(0 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			ret = buf.write(ar,buf.freeLen());
			assert(3 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			assert(ret);
			ret = buf.write(ar,0);
			assert(3 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			assert(!ret);	
			ret = buf.read(out, buf.len());
			assert(0 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			assert(ret);
			assert( 1==out[0] && 2==out[1] && 3==out[2] );

			assert(buf.empty());
			assert(!buf.full());
			ret = buf.write(ar,3);
			assert(ret);
			assert(3 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			buf.push(4);
			assert(3 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			ret = buf.read(out, buf.len());
			assert(0 == buf.len() && 3 == (buf.len()+buf.freeLen()));
			assert(ret);
			assert( 1==out[0] && 2==out[1] && 3==out[2] );

			assert(buf.empty());
			ret = buf.write(ar,3);
			assert(ret);
			ret = buf.read(out, 2);
			assert(ret);
			assert(1== buf.len() && 3 == (buf.len()+buf.freeLen()));
			assert( 1==out[0] && 2==out[1]);
			ret = buf.read(out, 1);
			assert(ret);
			assert(0== buf.len() && 3 == (buf.len()+buf.freeLen()));
			assert( 3==out[0] );
			ret = buf.read(out, 1);
			assert(!ret);
			assert(0== buf.len() && 3 == (buf.len()+buf.freeLen()));
		}
		{
			CircleBuf<uint32, 5> buf;

			uint32 out[10000+100]={0};
			uint32 offset = 0;
			for (uint32 index=0; index<10000; )
			{
				uint32 in[10];
				uint32 write_len = rand()%6;
				uint32 read_len = rand()%6;
				if (buf.freeLen()>=write_len)
				{
					for (uint32 i=0; i<write_len; ++i)
					{
						in[i]=index++; 
					}
                    if (0 == write_len)
                    {
                        assert(!buf.write(in, write_len));
                    }
                    else
                    {
					    assert(buf.write(in, write_len));
                    }
				}

				if (buf.len()>=read_len)
				{
                    if (0 == read_len)
                    { 
                        assert(!buf.read(&out[offset], read_len));
                    }
                    else
                    {
					    assert(buf.read(&out[offset], read_len));
                    }
					offset+=read_len;
				}
			}

			if (buf.len() > 0)
			{
				assert(buf.read(&out[offset], buf.len()));
			}

			for (uint32 i=0; i<10000; ++i)
			{
				if(out[i]!=i)
				{
					assert(false);
				}
			}
		}

	}
	//test getWillRead getWillWrite
	void testCircleBuf2()
	{
		CircleBuf<uint32, 5> buf;

		uint32 out[10000+100]={0};
		uint32 offset = 0;
		for (uint32 index=0; index<10000; )
		{
			uint32 in[10];
			uint32 write_len = rand()%6;
			uint32 read_len = rand()%6;

			uint32 *p_will_write=NULL;
            uint32 max_len = 0;
            p_will_write = buf.getCanContinueWrite(max_len);
            if(write_len > max_len)
            {
                write_len =  max_len;
            }
            for (uint32 i=0; i<write_len; ++i)
            {
                in[i]=index++; 
            }
            if (0 == max_len)
            {
                assert(NULL == p_will_write);
            }
            if(write_len>0)
            {
                assert(NULL != p_will_write);
                memcpy(p_will_write, in, sizeof(uint32)*write_len);
                assert(buf.finishWillWrite(write_len));
            }


            const uint32 *p_will_read=buf.getCanContinueRead(max_len);
            if(read_len > max_len)
            {
                read_len =  max_len;
            }
            if ( 0 == max_len)
            {
                assert(NULL == p_will_read);
            }
            if (read_len > 0)
            {
                assert(NULL != p_will_read);
                memcpy(&out[offset], p_will_read, sizeof(uint32)*read_len);
                buf.finishWillRead(read_len);
                offset+=read_len;
            }
		}


        if (0 != buf.len())
        {
            assert(buf.read(&out[offset], buf.len()));
        }
		for (uint32 i=0; i<10000; ++i)
		{
			if(out[i]!=i)
			{
				assert(false);
			}
		}
	}
	CircleBuf<uint32, 10> g_buf;
	vector<uint32> g_vec;
	struct MyThreed : public Thread
	{
		uint32 id;
		bool m_run;
		virtual void run()
		{
			uint32 in=1;
			while (m_run)
			{
				if (1 == id) //write
				{
					if (!g_buf.full())
					{
						g_buf.push(in++);
					}
				}
				else//read
				{
					if (!g_buf.empty())
					{
						uint32 t = 0;
						g_buf.pop(t);
						g_vec.push_back(t);
					}
				}
			}
			//Info("out run;\n");
		}
	};
	MyThreed g_thread_read, g_thread_write;
	void testCircleBufThread()
	{
		{
			Info("test thread ,pls wait 10s");
			g_thread_write.m_run = g_thread_read.m_run = true;
			g_thread_read.id = 2;
			g_thread_write.id = 1;
			g_thread_read.start();
			g_thread_write.start();
			sleep(10000);
			g_thread_write.m_run = g_thread_read.m_run = false;
			sleep(100);
		}

		uint32 max = g_vec.size();
		for (uint32 i=0; i<max-1; ++i)
		{
			assert(g_vec[i]==g_vec[i+1]-1);
		}
		printf("read buf cnt=%d\n", max);
	}
	void testCirMsgQue()
	{
		char buf[100 + sizeof(CirMsgQueMem)]; //CirMsgQueMem::buf[101] ,实际可用 100
		CirMsgQue::InitMemory(buf, sizeof(buf));
		CirMsgQue que;
		que.Init(buf, su::ArrayLen(buf));
		uint32 total_free_len = que.freeLen();
		//uint32 head_len = sizeof(CirMsgQueMem)-1;
		assert(total_free_len == su::ArrayLen(buf) - sizeof(CirMsgQueMem));

		char in_data1[] = "123";
		char in_data2[] = "4567";
		char r[500] = {};
		assert(que.Write(in_data1, 3));
		assert(que.Read(r, su::ArrayLen(r)));
		r[3] = 0; assert(string(r) == "123");

		assert(que.Write(in_data1, 3));
		assert(que.Write(in_data2, 4));
		assert(que.Read(r, su::ArrayLen(r)));
		r[3] = 0; assert(string(r) == "123");
		assert(que.Read(r, su::ArrayLen(r)));
		r[4] = 0; assert(string(r) == "4567");

		assert(false == que.Read(r, su::ArrayLen(r)));

		//测试写满的情况
		assert(0 == que.len());
		assert(total_free_len == que.freeLen());
		assert(que.Write(in_data1, (total_free_len - sizeof(uint32)))); //full_len = msg_len+head_len  -- msg_len = full_len - len_size
		assert(0 == que.freeLen());
		assert(0 == que.CanWriteLen());
		assert(false == que.Write(in_data1, 1));

		//末尾分界点读写情况
		assert(que.Read(r, su::ArrayLen(r)));
		assert(que.Write(in_data1, (total_free_len - sizeof(uint32)-2))); //full_len = msg_len+head_len  -- msg_len = full_len - len_size
		assert(2 == que.freeLen());
		assert(0 == que.CanWriteLen());
		assert(false == que.Write(in_data1, 1));
		assert(que.Read(r, su::ArrayLen(r)));

		assert(que.Write(in_data1, (total_free_len - sizeof(uint32)-5))); //full_len = msg_len+head_len  -- msg_len = full_len - len_size
		assert(5 == que.freeLen());
		assert(1 == que.CanWriteLen());
		assert(true == que.Write(in_data1, 1));
		assert(0 == que.freeLen());
		assert(0 == que.CanWriteLen());
		assert(que.Read(r, su::ArrayLen(r)));
		assert(total_free_len - 5 == que.freeLen());
		assert(que.Read(r, su::ArrayLen(r)));
		assert(total_free_len == que.freeLen());

		CirMsgQue::InitMemory(buf, su::ArrayLen(buf));
		assert(que.Write(in_data1, 6)); //随便数组前填10个。 msg_len = 10 - 4
		assert(que.Write(in_data1, (que.freeLen() - sizeof(uint32)-2))); //full_len = msg_len+head_len  -- msg_len = full_len - len_size
		assert(2 == que.freeLen());
		assert(que.Read(r, su::ArrayLen(r)));
		assert(2+6+4 == que.freeLen());
		//写分界点
		assert(que.Write(in_data2, 4));
		assert(que.Read(r, su::ArrayLen(r)));
		assert(que.Read(r, su::ArrayLen(r)));
		r[4] = 0; assert(string(r) == "4567");

	}

	char g_msg_buf[sizeof(CirMsgQueMem)+16];
	vector<uint8> g_msg_vec;
	struct CirMsgQueThread : public Thread
	{
		uint32 id;
		bool m_run;
		virtual void run()
		{
			CirMsgQue que;
			que.Init(g_msg_buf, sizeof(g_msg_buf));
			//uint32 max_space_size = sizeof(g_msg_buf)-sizeof(CirMsgQueMem)+1;

			uint32 in = 1;
			while (m_run)
			{
				if (1 == id) //write
				{
					uint32 write_len = wyl::randBetween(1, 9);
					if ( que.CanWriteLen() >= write_len)
					{
						char buf[20];
						for (uint32 i = 0; i < write_len; ++i)
						{
							buf[i] = in++;
						}
						assert(que.Write(buf, write_len));
					}
				}
				else//read
				{
					if (!que.empty())
					{
						char buf[20];
						uint32 read_len = que.Read(buf, sizeof(buf));
						if (0 == read_len)
						{
							return;
						}
						assert(read_len);
						for (uint32 i = 0; i < read_len; ++i)
						{
							g_msg_vec.push_back(buf[i]);
						}
					}
				}
			}
			//loop end
			if (1 == id) //write
			{
			}
			else//read
			{

			}
			//Info("out run;\n");
		}
	};
	CirMsgQueThread g_read_thread, g_write_thread;
	void testCirMsgQueThread()
	{
		CirMsgQue::InitMemory(g_msg_buf, sizeof(g_msg_buf));
		{
			Info("test testCirMsgQueThread ,pls wait 10s");
			g_write_thread.m_run = g_read_thread.m_run = true;
			g_read_thread.id = 2;
			g_write_thread.id = 1;
			g_read_thread.start();
			g_write_thread.start();
			sleep(1000*10);
			g_write_thread.m_run = g_read_thread.m_run = false;
			sleep(1000);
		}


		uint32 max = g_msg_vec.size();
		for (uint32 i = 0; i < max-1; ++i)
		{
			assert(g_msg_vec[i] == (uint8)(g_msg_vec[i + 1] - 1));
		}
		printf("read msg buf cnt=%d\n", max);
	}
} //namespace

void testUtility()
{
	for (uint32 i = 0; i < 10000*1000
		; ++i)
	{
		wyl::InitSeed();
		testCircleBuf();
		testCircleBuf2();
		testCircleBufThread();
		testCirMsgQue();
		testCirMsgQueThread();
	}
	Info("done");
}










