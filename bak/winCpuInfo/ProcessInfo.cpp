/*
filename:ProcessInfo.cpp
brief: 

version:1.0
author:	leo
date:	2012,Nov12
*/


//#include "stdafx.h"
#include "ProcessInfo.h"
#include "SysInfo.h"


CProcessInfo::CProcessInfo():
m_hProcess(GetCurrentProcess())
{
	m_iDProcess = GetProcessId(m_hProcess);
	InitMember();
}


CProcessInfo::CProcessInfo(DWORD dwProcessId)
{
	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessId);
	m_iDProcess = dwProcessId;
	InitMember();
}
CProcessInfo::~CProcessInfo(void)
{
}

void CProcessInfo::InitMember()
{
	FILETIME userTime,kernalTime;
	TIME *pLast = &m_lastProcessTime;

	pLast->cpuTime =m_sysInfo.GetCpuKernelTime();
	m_sysInfo.GetProcessCpuTime(m_hProcess,&userTime,&kernalTime);
	pLast->kernalTime = FileTime2Utc(&kernalTime);
	pLast->userTime = FileTime2Utc(&userTime);
}

FILETIME CProcessInfo::GetKernelTime()
{
	FILETIME fUserTime,fKernalTime;

	m_sysInfo.GetProcessCpuTime(m_hProcess,&fUserTime,&fKernalTime);
	return fKernalTime;
}

FILETIME CProcessInfo::GetUserTime()
{
	FILETIME fUserTime,fKernalTime;

	m_sysInfo.GetProcessCpuTime(m_hProcess,&fUserTime,&fKernalTime);
	return fUserTime;
}

FILETIME CProcessInfo::GetCpuTime()
{
	int64_t cpuTime;
	FILETIME t;

	cpuTime = m_sysInfo.GetCpuKernelTime();
	

	t.dwLowDateTime = (DWORD)cpuTime;
	t.dwHighDateTime = (DWORD)((0xffffffff00000000&cpuTime)>>32);
	return t;
}

double CProcessInfo::GetCpuRate()
{
	int64_t cpuTime;
	int64_t kernalTime;
	int64_t userTime;
	FILETIME fUserTime,fKernalTime;
	TIME *pLast = &m_lastProcessTime;

	cpuTime =m_sysInfo.GetCpuKernelTime();
	m_sysInfo.GetProcessCpuTime(m_hProcess,&fUserTime,&fKernalTime);
	kernalTime = FileTime2Utc(&fKernalTime);
	userTime = FileTime2Utc(&fUserTime);

	int64_t totalInterval = (kernalTime - pLast->kernalTime) + (userTime - pLast->userTime);
	int64_t cpuInterval = cpuTime - pLast->cpuTime;
	if(0 == cpuInterval)
	{
		return 0;
	}
	int64_t i = (totalInterval*100)/(cpuInterval);
	
	ULONG kernalNum = m_sysInfo.GetProcessorNumber();
	i = i/kernalNum;

	pLast->cpuTime = cpuTime;
	pLast->kernalTime = kernalTime;
	pLast->userTime = userTime;

	return static_cast<double>(i);
}

double CProcessInfo::GetThreadCpuRate(DWORD threadId)
{
	
	int64_t cpuTime;
	int64_t kernalTime;
	int64_t userTime;
	FILETIME fUserTime,fKernalTime;
	TIME *pLast = &m_lastThreadTime[threadId];
	HANDLE hThread =  OpenThread(THREAD_ALL_ACCESS,false,threadId);

	cpuTime =m_sysInfo.GetCpuKernelTime();
	m_sysInfo.GetThreadCpuTime(hThread,&fUserTime,&fKernalTime);
	kernalTime = FileTime2Utc(&fKernalTime);
	userTime = FileTime2Utc(&fUserTime);

	int64_t totalInterval = (kernalTime - pLast->kernalTime) + (userTime - pLast->userTime);
	int64_t cpuInterval = cpuTime - pLast->cpuTime;
	if(0 == cpuInterval)
	{
		return 0;
	}
	int64_t i = (totalInterval*100)/(cpuInterval);
	ULONG kernalNum = m_sysInfo.GetProcessorNumber();
	i = i/kernalNum;
	
	pLast->cpuTime = cpuTime;
	pLast->kernalTime = kernalTime;
	pLast->userTime = userTime;

	CloseHandle(hThread);
	return static_cast<double>(i);
}

ULONG CProcessInfo::GetCurrentMemorySize()
{
	return m_sysInfo.GetCurrentMemorySize(m_hProcess);
}

bool CProcessInfo::GetAllThreadId(DWORD *pIdBuf, UINT len, UINT *returnLen)
{
	return m_sysInfo.GetAllThreadId(m_iDProcess, pIdBuf, len, returnLen);
}