/*
brief:读取简单文本格式配置，不需要第三方库，可以快速加入工程使用。
配置文件格式1：name要求C语言命名风格，value任意字符串
name=value
name = 3
配置文件格式2：数组格式如下：(值不支持有,=字符)
name:v0,v1,v2 ...
*/
#pragma once
#include "typedef.h"
#include "cnt_typedef.h"
#include <utility>

class Config
{
public:
    bool init(const char *file_name);

    const char *GetStr(const char *name);
    int GetInt(const char *name);
    uint64 GetUint64(const char *name);
    void GetStrArray(const char *name, VecStr &vec_str);
    void GetIntArray(const char *name, VecInt &vec_int);
    //获取重复变量名，给警告，通常填错了。
    const VecStr &GetRepeatName();
private:
    Str2Str m_str_2_str;
    Str2Str m_str_2_array;
    VecStr m_repeat_str;
};