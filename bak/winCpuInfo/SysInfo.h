/*
filename:SysInfo.cpp
brief: Get system information

version:1.0
author:	leo
date:	2012,Nov12
*/
#pragma once
#include <windows.h>
#include <WinDef.h>
#include "SysInfoDef.h"

typedef long long           int64_t;
typedef unsigned long long  uint64_t;

uint64_t FileTime2Utc(const FILETIME* ftime);

class CSysInfo
{
public:
	CSysInfo(void);
	~CSysInfo(void);

	//@brief:get kernal amount,return 0 if false
	ULONG GetProcessorNumber();

	//@brief:get kernal time, 和进程对CPU占用率无关，每固定间隔得到的时间差是固定的。
	uint64_t GetCpuKernelTime();

	//@brief:get processe cpu time
	bool CSysInfo::GetProcessCpuTime(HANDLE hProcess, FILETIME *lpUserTime, FILETIME *lpKernelTime);

	//@brief:thread cpu time
	bool GetThreadCpuTime(HANDLE hThread, FILETIME *lpUserTime, FILETIME *lpKernelTime);
	
	//@brief:get memory Size
	ULONG GetCurrentMemorySize(HANDLE hProcess);

	/*
	@brief:get all thread id in one process.
	@parameters:
		DWORD iDProcess, ----
		DWORD *pIdBuf,  ----[out] Points to a buffer where the thread id array is to be returned
		UINT len,			---  array length of the DWORD *pIdBuf 
		UINT *returnLen		--- [out]  return array length
	@return:
		return false when fail.
	*/
	bool GetAllThreadId(DWORD iDProcess, DWORD *pIdBuf, UINT len, UINT *returnLen=NULL);

private:
	/*
		@brief:get point process infomation
	*/
	bool GetProcessInfoData(HANDLE hProcess, SYSTEM_PROCESSES *buf, UINT size, SYSTEM_PROCESSES **returnBufAddr);
private:
	typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);
	PROCNTQSI m_QuerySystemInfo;
	
};
