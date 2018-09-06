
#include "read_cfg.h"
#include <fstream>
#include "string_tool.h"

using namespace std;

bool Config::init( const char *file_name )
{
    ifstream file;
    
    file.open(file_name, ios::in);
    if (!file)
    {
        printf("open file [%s] fail", file_name);
        return false;
    }

    string buf;
    while(getline(file,buf))
    {
        VecStr vec_str;
        StringTool::split(buf, '=', vec_str);
        if (2 == vec_str.size())
        {
            StringTool::erase(vec_str[0], ' ');
            Str2Str::const_iterator it=m_str_2_str.find(vec_str[0]);
            if (it!=m_str_2_str.end())
            {
                m_repeat_str.push_back(vec_str[0]);
            }
            m_str_2_str[vec_str[0]]=vec_str[1];
            continue;
        }
        StringTool::split(buf, ':', vec_str);
        if (2 == vec_str.size())
        {
            StringTool::erase(vec_str[0], ' ');
            Str2Str::const_iterator it=m_str_2_array.find(vec_str[0]);
            if (it!=m_str_2_array.end())
            {
                m_repeat_str.push_back(vec_str[0]);
            }
            m_str_2_array[vec_str[0]]=vec_str[1];
            continue;
        }
    };

    file.close();
    return true;
}

const char * Config::GetStr( const char *name )
{
    Str2Str::const_iterator it = m_str_2_str.find(name);
    if (it == m_str_2_str.end())
    {
        return "";
    }
    return (it->second).c_str();
}

int Config::GetInt( const char *name )
{
    Str2Str::const_iterator it = m_str_2_str.find(name);
    if (it == m_str_2_str.end())
    {
        return 0;
    }

    return atoi((it->second).c_str());
}

uint64 Config::GetUint64( const char *name )
{
    Str2Str::const_iterator it = m_str_2_str.find(name);
    if (it == m_str_2_str.end())
    {
        return 0;
    }
    uint64 num=0;
#ifdef WIN32
	sscanf_s((it->second).c_str(), "%lld", &num);
#else
	sscanf((it->second).c_str(), "%lld", &num);
#endif //#ifdef WIN32
    return num;
}

void Config::GetIntArray( const char *name, VecInt &vec_int )
{
    vec_int.clear();
    Str2Str::const_iterator it = m_str_2_array.find(name);
    if (it == m_str_2_array.end())
    {
        return;
    }

    VecStr vec_str;
    StringTool::split(it->second, ',', vec_str);
    for(VecStr::const_iterator it=vec_str.begin(); it!=vec_str.end(); ++it)
    {
        const string &s = *it;
        int i = atoi(s.c_str());
        vec_int.push_back(i);
    }
}

void Config::GetStrArray( const char *name, VecStr &vec_str )
{
    vec_str.clear();
    Str2Str::const_iterator it = m_str_2_array.find(name);
    if (it == m_str_2_array.end())
    {
        return;
    }

    StringTool::split(it->second, ',', vec_str);
}

const VecStr & Config::GetRepeatName()
{
    return m_repeat_str;
}
