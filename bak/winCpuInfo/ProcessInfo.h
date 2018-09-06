/*
filename:ProcessInfo.h
brief: get process information
remark: requir Psapi.lib 

version:1.0
author:	leo
date:	2012,Nov12
*/

#pragma once
#include <windows.h>
#include <WinDef.h>
#include "SysInfo.h"
#include <map>
#pragma comment ( lib , "Psapi.lib" ) 

class CProcessInfo
{
public:
	CProcessInfo();
	CProcessInfo(DWORD dwProcessId);
	~CProcessInfo(void);

	/*
	@brief:get cpu rate of  process,
			the detect interval is from last call this function to current call.
	@return: return value == 100 means 100%
	*/
	double GetCpuRate();

	/*
	@brief:get cpu rate of thread, 
			the detect interval is from last call this function to current call.
	@return: return value == 100 means 100%
	*/
    double GetThreadCpuRate(DWORD threadId);

	/*
	@brief:get process current memory size
	*/
	ULONG GetCurrentMemorySize();

	/*
	@remark:
	havn't complete
	*/
	ULONG GetActiveSocketNumber(DWORD dwProcessId);

	/*
	@brief:get all thread id in one process.
	@parameters:
		DWORD *pIdBuf,  ----[out] Points to a buffer where the thread id array is to be returned
		UINT len,			---  array length of the DWORD *pIdBuf 
		UINT *returnLen		--- [out]  return array length
	@return:
		return false when fail.
	*/
	bool GetAllThreadId(DWORD *pIdBuf, UINT len, UINT *returnLen=NULL);

	FILETIME  GetKernelTime();

	FILETIME  GetUserTime();

	/*
	@brief:得到CPU时钟，和进程对CPU占用率无关，每固定间隔得到的时间差是固定的。
	*/
	FILETIME  GetCpuTime();

private:
	void InitMember();

private:
	typedef struct{
		int64_t cpuTime;
		int64_t kernalTime;
		int64_t userTime;
	}TIME;
	CSysInfo m_sysInfo;
	DWORD m_iDProcess;
	HANDLE m_hProcess;
	TIME m_lastProcessTime;
	std::map<DWORD,TIME> m_lastThreadTime;
	
	
};
