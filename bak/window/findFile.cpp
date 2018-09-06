

#pragma once

#include "findFile.h"
#include <string>
#include <iostream>

using namespace std;

extern string g_s;
void PrintFileName( const char *pName )
{
	cout << pName << endl;
}	
void testFindFile()
{
	findFile(NULL, "*.txt", PrintFileName);
}

void HandleFileName( const char *pName )
{
	
}

void findFile(const char *pPath, const char *pFind, PHandleFileName handle)
{
	CString path;
	if (NULL == pPath)
	{
		path += ".\\";
	}
	else
	{
		path+=pPath;
	}
	path+="\\";
	if (NULL == pFind)
	{
		path += "*";
	}
	else
	{
		path+=pFind;
	}

	CFileFind f; 
	BOOL ret=f.FindFile(path); 
	while(ret) 
	{ 
		ret=f.FindNextFile();  
		if (f.IsDots())
		{
			continue;
		}

		CString filePath = f.GetFilePath();
		if( f.IsDirectory())
		{
			CString name=f.GetFileName();
			{
				USES_CONVERSION;  //先调用此宏定义W2A需要使用到的变量   
				findFile((char *)W2A(filePath), pFind, handle);
			}
		}
		else
		{
			CString name=f.GetFileName();
			{
				USES_CONVERSION;  //先调用此宏定义W2A需要使用到的变量   
				handle((char *)W2A(filePath));//调用W2A宏
			}

		}
	} 
}