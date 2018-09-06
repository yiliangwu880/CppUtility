/*
filename:SysInfo.cpp
brief: 

version:1.0
author:	leo
date:	2012,Nov12
*/

//#include "stdafx.h"
#include "SysInfo.h"
#include <Tlhelp32.h>

#include <Psapi.h>
#include <assert.h>
//#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))

/// 时间转换
uint64_t FileTime2Utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

CSysInfo::CSysInfo(void)
{
	m_QuerySystemInfo = (PROCNTQSI)GetProcAddress(GetModuleHandleA(LPCSTR("ntdll")),
		LPCSTR("NtQuerySystemInformation"));
}

CSysInfo::~CSysInfo(void)
{
}

ULONG CSysInfo::GetProcessorNumber()
{
	SYSTEM_BASIC_INFORMATION_SELF d;

	LONG status = m_QuerySystemInfo(SystemBasicInformation,&d,sizeof(d),NULL);
	if (status != NO_ERROR)
	{
		return 0;
	}
	return d.bKeNumberProcessors;
}

uint64_t CSysInfo::GetCpuKernelTime()
{
	/*
	//method 1 have unknow error!!
	SYSTEM_PERFORMANCE_INFORMATION_SELF d;

	LONG status = m_QuerySystemInfo(SystemPerformanceInformation,&d,sizeof(d),NULL);
	if (status != NO_ERROR)
	{
		assert(0);//havn't know how to handle
		return 0;
	}
	return d.IdleTime.QuadPart;
*/
	////method 2
	FILETIME t;
	GetSystemTimeAsFileTime(&t);
	return FileTime2Utc(&t);

}

bool CSysInfo::GetProcessCpuTime(HANDLE hProcess, FILETIME *lpUserTime, FILETIME *lpKernelTime)
{
	FILETIME t1,t2;

	if(false == GetProcessTimes(hProcess,&t1, &t2, lpKernelTime, lpUserTime))
	{
		return false;
	}

	return true;
}

bool CSysInfo::GetThreadCpuTime(HANDLE hThread, FILETIME *lpUserTime, FILETIME *lpKernelTime)
{
	FILETIME t1,t2;

	if(false == GetThreadTimes(hThread,&t1, &t2, lpKernelTime, lpUserTime))
	{
		return false;
	}

	return true;
}

ULONG CSysInfo::GetCurrentMemorySize(HANDLE hProcess)
{
	PROCESS_MEMORY_COUNTERS d;
	if(false == GetProcessMemoryInfo(hProcess,&d,sizeof(d)))
	{
		assert(0);//havn't know how to handle
		return 0;
	}

	return static_cast<ULONG>(d.WorkingSetSize);
}

bool CSysInfo::GetAllThreadId(DWORD iDProcess, DWORD *pIdBuf, UINT len, UINT *returnLen)
{
	HANDLE hSnapThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, iDProcess);  

	if (INVALID_HANDLE_VALUE != hSnapThread)   
	{  
		THREADENTRY32 te32 = {sizeof(te32)};  
		if (Thread32First(hSnapThread, &te32))   
		{   
			DWORD cnt = 0;  
			UINT noUse;
			if(NULL == returnLen)
			{
				returnLen = &noUse;
			}
			*returnLen = 0;
			do {  
				if (iDProcess == te32.th32OwnerProcessID)  
				{ 
					*(pIdBuf+cnt) = te32.th32ThreadID;
					cnt++;  (*returnLen)++;
					if (cnt > len)
					{
						break;
					}
				} 
			} while (Thread32Next(hSnapThread, &te32)); 
		} 

		CloseHandle(hSnapThread); 
	}
	return false;
}

bool CSysInfo::GetProcessInfoData(HANDLE hProcess, SYSTEM_PROCESSES *buf, UINT size, SYSTEM_PROCESSES **returnBufAddr)
{
	return false;//未能实现
	/*
	SYSTEM_PROCESSES *p = buf;
	ULONG allSize=0;

	LONG status = m_QuerySystemInfo(SystemProcessInformation,buf,size,&allSize);
	if (status != NO_ERROR)
	{
		assert(0);//havn't know how to handle
		return false;
	}

	for (;;)
	{
		if(hProcess == OpenProcess(PROCESS_ALL_ACCESS, false, p->ProcessId))
		{
			*returnBufAddr = p;
			return true;
		}
		if(0 == p->NextEntryDelta)
		{
			break;
		}
		p = p+p->NextEntryDelta;
	}
	return false;
	*/
}
