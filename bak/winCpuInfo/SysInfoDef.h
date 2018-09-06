/*
filename:SysInfo.cpp
brief: define information for funtion"NtQuerySystemInformation" or system information
remark: �ṹ������ϳ�Ϯ��δȷ��ȫ����ȷ��

version:1.0
author:	leo
date:	2012,Nov12
*/

#pragma once
#include <windows.h>
#include <WinDef.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 //win xp
#endif
#include <Winternl.h>


typedef LONG NTSTATUS;

typedef struct _LSA_UNICODE_STRING
{
	USHORT  Length;
	USHORT  MaximumLength;
	PWSTR   Buffer;
}LSA_UNICODE_STRING,*PLSA_UNICODE_STRING;
//typedef LSA_UNICODE_STRING UNICODE_STRING, *PUNICODE_STRING;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
}CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

typedef LONG KPRIORITY;

typedef struct _VM_COUNTERS
{
	ULONG PeakVirtualSize;
	ULONG VirtualSize;
	ULONG PageFaultCount;
	ULONG PeakWorkingSetSize;
	ULONG WorkingSetSize;
	ULONG QuotaPeakPagedPoolUsage;
	ULONG QuotaPagedPoolUsage;
	ULONG QuotaPeakNonPagedPoolUsage;
	ULONG QuotaNonPagedPoolUsage;
	ULONG PagefileUsage;
	ULONG PeakPagefileUsage;
}VM_COUNTERS,*PVM_COUNTERS;

typedef struct
{
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
}IO_COUNTERS_SELF;


typedef enum _THREAD_STATE
{
	StateInitialized,
	StateReady,
	StateRunning,
	StateStandby,
	StateTerminated,
	StateWait,
	StateTransition,
	StateUnknown
}THREAD_STATE;

typedef enum _KWAIT_REASON
{
	Executive,
	FreePage,
	PageIn,
	PoolAllocation,
	DelayExecution,
	Suspended,
	UserRequest,
	WrExecutive,
	WrFreePage,
	WrPageIn,
	WrPoolAllocation,
	WrDelayExecution,
	WrSuspended,
	WrUserRequest,
	WrEventPair,
	WrQueue,
	WrLpcReceive,
	WrLpcReply,
	WrVertualMemory,
	WrPageOut,
	WrRendezvous,
	Spare2,
	Spare3,
	Spare4,
	Spare5,
	Spare6,
	WrKernel
}KWAIT_REASON;
typedef struct
{
	LARGE_INTEGER KernelTime;               //CPU�ں�ģʽʹ��ʱ�䣻
	LARGE_INTEGER UserTime;                 //CPU�û�ģʽʹ��ʱ�䣻
	LARGE_INTEGER CreateTime;               //�̴߳���ʱ�䣻
	ULONG         WaitTime;                 //�ȴ�ʱ�䣻
	PVOID         StartAddress;             //�߳̿�ʼ�������ַ��
	CLIENT_ID     ClientId;                 //�̱߳�ʶ����
	KPRIORITY     Priority;                 //�߳����ȼ���
	KPRIORITY     BasePriority;             //�������ȼ���
	ULONG         ContextSwitchCount;       //�����л���Ŀ��
	THREAD_STATE  State;                    //��ǰ״̬��
	KWAIT_REASON  WaitReason;               //�ȴ�ԭ��
}SYSTEM_THREADS;



typedef struct _SYSTEM_PROCESSES
{
	ULONG          NextEntryDelta;
	ULONG          ThreadCount;
	ULONG          Reserved1[6];
	LARGE_INTEGER  CreateTime;
	LARGE_INTEGER  UserTime;
	LARGE_INTEGER  KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY      BasePriority;
	ULONG          ProcessId;
	ULONG          InheritedFromProcessId;
	ULONG          HandleCount;
	ULONG          Reserved2[2];
	VM_COUNTERS    VmCounters;
	IO_COUNTERS_SELF    IoCounters;
	SYSTEM_THREADS Threads[1];
}SYSTEM_PROCESSES,*PSYSTEM_PROCESSES;


/////////////////////////////////////////////////////////////////

typedef struct
{
	DWORD dwUnknown1;
	ULONG uKeMaximumIncrement;
	ULONG uPageSize;
	ULONG uMmNumberOfPhysicalPages;
	ULONG uMmLowestPhysicalPage;
	ULONG uMmHighestPhysicalPage;
	ULONG uAllocationGranularity;
	PVOID pLowestUserAddress;
	PVOID pMmHighestUserAddress;
	ULONG uKeActiveProcessors;
	BYTE bKeNumberProcessors;
	BYTE bUnknown2;
	WORD wUnknown3;
} SYSTEM_BASIC_INFORMATION_SELF;


typedef struct
{
	ULONG          NextEntryDelta;          //���ɽṹ���е�ƫ������
	ULONG          ThreadCount;             //�߳���Ŀ��
	ULONG          Reserved1[6];          
	LARGE_INTEGER  CreateTime;              //����ʱ�䣻
	LARGE_INTEGER  UserTime;                //�û�ģʽ(Ring 3)��CPUʱ�䣻
	LARGE_INTEGER  KernelTime;              //�ں�ģʽ(Ring 0)��CPUʱ�䣻
	UNICODE_STRING ProcessName;             //�������ƣ�
	KPRIORITY      BasePriority;            //��������Ȩ��
	ULONG          ProcessId;               //���̱�ʶ����
	ULONG          InheritedFromProcessId;  //�����̵ı�ʶ����
	ULONG          HandleCount;             //�����Ŀ��
	ULONG          Reserved2[2];
	VM_COUNTERS    VmCounters;              //����洢���Ľṹ�����£�
	IO_COUNTERS_SELF    IoCounters;              //IO�����ṹ�����£�
	SYSTEM_THREADS Threads[1];              //��������̵߳Ľṹ���飬���£�
}SYSTEM_PROCESSES_SELF;



typedef struct
{
	ULONG PeakVirtualSize;                  //����洢��ֵ��С��
	ULONG VirtualSize;                      //����洢��С��
	ULONG PageFaultCount;                   //ҳ������Ŀ��
	ULONG PeakWorkingSetSize;               //��������ֵ��С��
	ULONG WorkingSetSize;                   //��������С��
	ULONG QuotaPeakPagedPoolUsage;          //��ҳ��ʹ������ֵ��
	ULONG QuotaPagedPoolUsage;              //��ҳ��ʹ����
	ULONG QuotaPeakNonPagedPoolUsage;       //�Ƿ�ҳ��ʹ������ֵ��
	ULONG QuotaNonPagedPoolUsage;           //�Ƿ�ҳ��ʹ����
	ULONG PagefileUsage;                    //ҳ�ļ�ʹ�������
	ULONG PeakPagefileUsage;                //ҳ�ļ�ʹ�÷�ֵ��
}VM_COUNTERS_SELF;


typedef struct
{
	LARGE_INTEGER  IdleTime;                    //CPU����ʱ�䣻
	LARGE_INTEGER  ReadTransferCount;           //I/O��������Ŀ��
	LARGE_INTEGER  WriteTransferCount;          //I/Oд������Ŀ��
	LARGE_INTEGER  OtherTransferCount;          //I/O����������Ŀ��
	ULONG          ReadOperationCount;          //I/O��������Ŀ��
	ULONG          WriteOperationCount;         //I/Oд������Ŀ��
	ULONG          OtherOperationCount;         //I/O��������������Ŀ��
	ULONG          AvailablePages;              //�ɻ�õ�ҳ��Ŀ��
	ULONG          TotalCommittedPages;         //�ܹ��ύҳ��Ŀ��
	ULONG          TotalCommitLimit;            //���ύҳ��Ŀ��
	ULONG          PeakCommitment;              //ҳ�ύ��ֵ��
	ULONG          PageFaults;                  //ҳ������Ŀ��
	ULONG          WriteCopyFaults;             //Copy-On-Write������Ŀ��
	ULONG          TransitionFaults;            //��ҳ������Ŀ��
	ULONG          Reserved1;
	ULONG          DemandZeroFaults;            //����0��������
	ULONG          PagesRead;                   //��ҳ��Ŀ��
	ULONG          PageReadIos;                 //��ҳI/O��������
	ULONG          Reserved2[2];
	ULONG          PagefilePagesWritten;        //��дҳ�ļ�ҳ����
	ULONG          PagefilePageWriteIos;        //��дҳ�ļ���������
	ULONG          MappedFilePagesWritten;      //��дӳ���ļ�ҳ����
	ULONG          MappedFileWriteIos;          //��дӳ���ļ���������
	ULONG          PagedPoolUsage;              //��ҳ��ʹ�ã�
	ULONG          NonPagedPoolUsage;           //�Ƿ�ҳ��ʹ�ã�
	ULONG          PagedPoolAllocs;             //��ҳ�ط��������
	ULONG          PagedPoolFrees;              //��ҳ���ͷ������
	ULONG          NonPagedPoolAllocs;          //�Ƿ�ҳ�ط��������
	ULONG          NonPagedPoolFress;           //�Ƿ�ҳ���ͷ������
	ULONG          TotalFreeSystemPtes;         //ϵͳҳ�����ͷ�������
	ULONG          SystemCodePage;              //����ϵͳ����ҳ����
	ULONG          TotalSystemDriverPages;      //�ɷ�ҳ��������ҳ����
	ULONG          TotalSystemCodePages;        //����ϵͳ����ҳ������
	ULONG          SmallNonPagedLookasideListAllocateHits; //
	ULONG          SmallPagedLookasideListAllocateHits;
	ULONG          Reserved3;                  
	ULONG          MmSystemCachePage;          //ϵͳ����ҳ����
	ULONG          PagedPoolPage;              //��ҳ��ҳ����
	ULONG          SystemDriverPage;           //�ɷ�ҳ����ҳ����
	ULONG          FastReadNoWait;             //�첽���ٶ���Ŀ��
	ULONG          FastReadWait;               //ͬ�����ٶ���Ŀ��
	ULONG          FastReadResourceMiss;       //���ٶ���Դ��ͻ����
	ULONG          FastReadNotPossible;        //���ٶ�ʧ������
	ULONG          FastMdlReadNoWait;          //�첽MDL���ٶ���Ŀ��
	ULONG          FastMdlReadWait;            //ͬ��MDL���ٶ���Ŀ��
	ULONG          FastMdlReadResourceMiss;    //MDL����Դ��ͻ����
	ULONG          FastMdlReadNotPossible;     //MDL��ʧ������
	ULONG          MapDataNoWait;              //�첽ӳ�����ݴ�����
	ULONG          MapDataWait;                //ͬ��ӳ�����ݴ�����
	ULONG          MapDataNoWaitMiss;          //�첽ӳ�����ݳ�ͻ������
	ULONG          MapDataWaitMiss;            //ͬ��ӳ�����ݳ�ͻ������
	ULONG          PinMappedDataCount;         //ǣ��ӳ��������Ŀ��
	ULONG          PinReadNoWait;              //ǣ���첽����Ŀ��
	ULONG          PinReadWait;                //ǣ��ͬ������Ŀ��
	ULONG          PinReadNoWaitMiss;          //ǣ���첽����ͻ��Ŀ��
	ULONG          PinReadWaitMiss;            //ǣ��ͬ������ͻ��Ŀ��
	ULONG          CopyReadNoWait;             //�첽������������
	ULONG          CopyReadWait;               //ͬ��������������
	ULONG          CopyReadNoWaitMiss;         //�첽���������ϴ�����
	ULONG          CopyReadWaitMiss;           //ͬ�����������ϴ�����
	ULONG          MdlReadNoWait;              //�첽MDL��������
	ULONG          MdlReadWait;                //ͬ��MDL��������
	ULONG          MdlReadNoWaitMiss;          //�첽MDL�����ϴ�����
	ULONG          MdlReadWaitMiss;            //ͬ��MDL�����ϴ�����
	ULONG          ReadAheadIos;               //��ǰ��������Ŀ��
	ULONG          LazyWriteIos;               //LAZYд������Ŀ��
	ULONG          LazyWritePages;             //LAZYдҳ�ļ���Ŀ��
	ULONG          DataFlushes;                //����ˢ�´�����
	ULONG          DataPages;                  //����ˢ��ҳ����
	ULONG          ContextSwitches;            //�����л���Ŀ��
	ULONG          FirstLevelTbFills;          //��һ�㻺������������
	ULONG          SecondLevelTbFills;         //�ڶ��㻺������������
	ULONG          SystemCall;                 //ϵͳ���ô�����
}SYSTEM_PERFORMANCE_INFORMATION_SELF;



typedef struct __SYSTEM_PROCESSOR_TIMES
{
	LARGE_INTEGER IdleTime;               //����ʱ�䣻
	LARGE_INTEGER KernelTime;             //�ں�ģʽʱ�䣻
	LARGE_INTEGER UserTime;               //�û�ģʽʱ�䣻
	LARGE_INTEGER DpcTime;                //�ӳٹ��̵���ʱ�䣻
	LARGE_INTEGER InterruptTime;          //�ж�ʱ�䣻
	ULONG         InterruptCount;         //�жϴ�����
}SYSTEM_PROCESSOR_TIMES,*PSYSTEM_PROCESSOR_TIMES;



typedef struct _SYSTEM_PAGEFILE_INFORMATION
{
	ULONG NetxEntryOffset;                //��һ���ṹ��ƫ������
	ULONG CurrentSize;                    //��ǰҳ�ļ���С��
	ULONG TotalUsed;                      //��ǰʹ�õ�ҳ�ļ�����
	ULONG PeakUsed;                       //��ǰʹ�õ�ҳ�ļ���ֵ����
	UNICODE_STRING FileName;              //ҳ�ļ����ļ����ƣ�
}SYSTEM_PAGEFILE_INFORMATION,*PSYSTEM_PAGEFILE_INFORMATION;



typedef struct _SYSTEM_CACHE_INFORMATION
{
	ULONG SystemCacheWsSize;              //���ٻ����С��
	ULONG SystemCacheWsPeakSize;          //���ٻ����ֵ��С��
	ULONG SystemCacheWsFaults;            //���ٻ���ҳ������Ŀ��
	ULONG SystemCacheWsMinimum;           //���ٻ�����Сҳ��С��
	ULONG SystemCacheWsMaximum;           //���ٻ������ҳ��С��
	ULONG TransitionSharedPages;          //����ҳ��Ŀ��
	ULONG TransitionSharedPagesPeak;      //����ҳ��ֵ��Ŀ��
	ULONG Reserved[2];
}SYSTEM_CACHE_INFORMATION,*PSYSTEM_CACHE_INFORMATION;


