/*
brief: find file
*/

#pragma once

#define _AFXDLL 
#include "stdafx.h"
#include <string>


void HandleFileName(const char *pName);

typedef void (*PHandleFileName)(const char *pName);

//�ݹ�Ŀ¼�����ļ�����
void findFile(const char *pPath,  const char *pFind, PHandleFileName handle);


