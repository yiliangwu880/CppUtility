/*
filename:SysInfo.cpp
brief: define information for funtion"NtQuerySystemInformation" or system information
remark: 结构体从网上抄袭，未确保全部正确。

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
	LARGE_INTEGER KernelTime;               //CPU内核模式使用时间；
	LARGE_INTEGER UserTime;                 //CPU用户模式使用时间；
	LARGE_INTEGER CreateTime;               //线程创建时间；
	ULONG         WaitTime;                 //等待时间；
	PVOID         StartAddress;             //线程开始的虚拟地址；
	CLIENT_ID     ClientId;                 //线程标识符；
	KPRIORITY     Priority;                 //线程优先级；
	KPRIORITY     BasePriority;             //基本优先级；
	ULONG         ContextSwitchCount;       //环境切换数目；
	THREAD_STATE  State;                    //当前状态；
	KWAIT_REASON  WaitReason;               //等待原因；
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
	ULONG          NextEntryDelta;          //构成结构序列的偏移量；
	ULONG          ThreadCount;             //线程数目；
	ULONG          Reserved1[6];          
	LARGE_INTEGER  CreateTime;              //创建时间；
	LARGE_INTEGER  UserTime;                //用户模式(Ring 3)的CPU时间；
	LARGE_INTEGER  KernelTime;              //内核模式(Ring 0)的CPU时间；
	UNICODE_STRING ProcessName;             //进程名称；
	KPRIORITY      BasePriority;            //进程优先权；
	ULONG          ProcessId;               //进程标识符；
	ULONG          InheritedFromProcessId;  //父进程的标识符；
	ULONG          HandleCount;             //句柄数目；
	ULONG          Reserved2[2];
	VM_COUNTERS    VmCounters;              //虚拟存储器的结构，见下；
	IO_COUNTERS_SELF    IoCounters;              //IO计数结构，见下；
	SYSTEM_THREADS Threads[1];              //进程相关线程的结构数组，见下；
}SYSTEM_PROCESSES_SELF;



typedef struct
{
	ULONG PeakVirtualSize;                  //虚拟存储峰值大小；
	ULONG VirtualSize;                      //虚拟存储大小；
	ULONG PageFaultCount;                   //页故障数目；
	ULONG PeakWorkingSetSize;               //工作集峰值大小；
	ULONG WorkingSetSize;                   //工作集大小；
	ULONG QuotaPeakPagedPoolUsage;          //分页池使用配额峰值；
	ULONG QuotaPagedPoolUsage;              //分页池使用配额；
	ULONG QuotaPeakNonPagedPoolUsage;       //非分页池使用配额峰值；
	ULONG QuotaNonPagedPoolUsage;           //非分页池使用配额；
	ULONG PagefileUsage;                    //页文件使用情况；
	ULONG PeakPagefileUsage;                //页文件使用峰值；
}VM_COUNTERS_SELF;


typedef struct
{
	LARGE_INTEGER  IdleTime;                    //CPU空闲时间；
	LARGE_INTEGER  ReadTransferCount;           //I/O读操作数目；
	LARGE_INTEGER  WriteTransferCount;          //I/O写操作数目；
	LARGE_INTEGER  OtherTransferCount;          //I/O其他操作数目；
	ULONG          ReadOperationCount;          //I/O读数据数目；
	ULONG          WriteOperationCount;         //I/O写数据数目；
	ULONG          OtherOperationCount;         //I/O其他操作数据数目；
	ULONG          AvailablePages;              //可获得的页数目；
	ULONG          TotalCommittedPages;         //总共提交页数目；
	ULONG          TotalCommitLimit;            //已提交页数目；
	ULONG          PeakCommitment;              //页提交峰值；
	ULONG          PageFaults;                  //页故障数目；
	ULONG          WriteCopyFaults;             //Copy-On-Write故障数目；
	ULONG          TransitionFaults;            //软页故障数目；
	ULONG          Reserved1;
	ULONG          DemandZeroFaults;            //需求0故障数；
	ULONG          PagesRead;                   //读页数目；
	ULONG          PageReadIos;                 //读页I/O操作数；
	ULONG          Reserved2[2];
	ULONG          PagefilePagesWritten;        //已写页文件页数；
	ULONG          PagefilePageWriteIos;        //已写页文件操作数；
	ULONG          MappedFilePagesWritten;      //已写映射文件页数；
	ULONG          MappedFileWriteIos;          //已写映射文件操作数；
	ULONG          PagedPoolUsage;              //分页池使用；
	ULONG          NonPagedPoolUsage;           //非分页池使用；
	ULONG          PagedPoolAllocs;             //分页池分配情况；
	ULONG          PagedPoolFrees;              //分页池释放情况；
	ULONG          NonPagedPoolAllocs;          //非分页池分配情况；
	ULONG          NonPagedPoolFress;           //非分页池释放情况；
	ULONG          TotalFreeSystemPtes;         //系统页表项释放总数；
	ULONG          SystemCodePage;              //操作系统代码页数；
	ULONG          TotalSystemDriverPages;      //可分页驱动程序页数；
	ULONG          TotalSystemCodePages;        //操作系统代码页总数；
	ULONG          SmallNonPagedLookasideListAllocateHits; //
	ULONG          SmallPagedLookasideListAllocateHits;
	ULONG          Reserved3;                  
	ULONG          MmSystemCachePage;          //系统缓存页数；
	ULONG          PagedPoolPage;              //分页池页数；
	ULONG          SystemDriverPage;           //可分页驱动页数；
	ULONG          FastReadNoWait;             //异步快速读数目；
	ULONG          FastReadWait;               //同步快速读数目；
	ULONG          FastReadResourceMiss;       //快速读资源冲突数；
	ULONG          FastReadNotPossible;        //快速读失败数；
	ULONG          FastMdlReadNoWait;          //异步MDL快速读数目；
	ULONG          FastMdlReadWait;            //同步MDL快速读数目；
	ULONG          FastMdlReadResourceMiss;    //MDL读资源冲突数；
	ULONG          FastMdlReadNotPossible;     //MDL读失败数；
	ULONG          MapDataNoWait;              //异步映射数据次数；
	ULONG          MapDataWait;                //同步映射数据次数；
	ULONG          MapDataNoWaitMiss;          //异步映射数据冲突次数；
	ULONG          MapDataWaitMiss;            //同步映射数据冲突次数；
	ULONG          PinMappedDataCount;         //牵制映射数据数目；
	ULONG          PinReadNoWait;              //牵制异步读数目；
	ULONG          PinReadWait;                //牵制同步读数目；
	ULONG          PinReadNoWaitMiss;          //牵制异步读冲突数目；
	ULONG          PinReadWaitMiss;            //牵制同步读冲突数目；
	ULONG          CopyReadNoWait;             //异步拷贝读次数；
	ULONG          CopyReadWait;               //同步拷贝读次数；
	ULONG          CopyReadNoWaitMiss;         //异步拷贝读故障次数；
	ULONG          CopyReadWaitMiss;           //同步拷贝读故障次数；
	ULONG          MdlReadNoWait;              //异步MDL读次数；
	ULONG          MdlReadWait;                //同步MDL读次数；
	ULONG          MdlReadNoWaitMiss;          //异步MDL读故障次数；
	ULONG          MdlReadWaitMiss;            //同步MDL读故障次数；
	ULONG          ReadAheadIos;               //向前读操作数目；
	ULONG          LazyWriteIos;               //LAZY写操作数目；
	ULONG          LazyWritePages;             //LAZY写页文件数目；
	ULONG          DataFlushes;                //缓存刷新次数；
	ULONG          DataPages;                  //缓存刷新页数；
	ULONG          ContextSwitches;            //环境切换数目；
	ULONG          FirstLevelTbFills;          //第一层缓冲区填充次数；
	ULONG          SecondLevelTbFills;         //第二层缓冲区填充次数；
	ULONG          SystemCall;                 //系统调用次数；
}SYSTEM_PERFORMANCE_INFORMATION_SELF;



typedef struct __SYSTEM_PROCESSOR_TIMES
{
	LARGE_INTEGER IdleTime;               //空闲时间；
	LARGE_INTEGER KernelTime;             //内核模式时间；
	LARGE_INTEGER UserTime;               //用户模式时间；
	LARGE_INTEGER DpcTime;                //延迟过程调用时间；
	LARGE_INTEGER InterruptTime;          //中断时间；
	ULONG         InterruptCount;         //中断次数；
}SYSTEM_PROCESSOR_TIMES,*PSYSTEM_PROCESSOR_TIMES;



typedef struct _SYSTEM_PAGEFILE_INFORMATION
{
	ULONG NetxEntryOffset;                //下一个结构的偏移量；
	ULONG CurrentSize;                    //当前页文件大小；
	ULONG TotalUsed;                      //当前使用的页文件数；
	ULONG PeakUsed;                       //当前使用的页文件峰值数；
	UNICODE_STRING FileName;              //页文件的文件名称；
}SYSTEM_PAGEFILE_INFORMATION,*PSYSTEM_PAGEFILE_INFORMATION;



typedef struct _SYSTEM_CACHE_INFORMATION
{
	ULONG SystemCacheWsSize;              //高速缓存大小；
	ULONG SystemCacheWsPeakSize;          //高速缓存峰值大小；
	ULONG SystemCacheWsFaults;            //高速缓存页故障数目；
	ULONG SystemCacheWsMinimum;           //高速缓存最小页大小；
	ULONG SystemCacheWsMaximum;           //高速缓存最大页大小；
	ULONG TransitionSharedPages;          //共享页数目；
	ULONG TransitionSharedPagesPeak;      //共享页峰值数目；
	ULONG Reserved[2];
}SYSTEM_CACHE_INFORMATION,*PSYSTEM_CACHE_INFORMATION;


