#include "string_tool.h"
#include <algorithm>

using namespace std;

void StringTool::unique(const VecStr& strList, VecStr& out)
{
    out.clear();
    out.reserve(strList.size());
    for(VecStr::const_iterator it_from = strList.begin();
        it_from != strList.end(); ++it_from)
    {
        bool repeat = false;
        for(VecStr::const_iterator it_to = out.begin(); 
            it_to != out.end(); ++it_to)
        {
            if(it_from->compare(*it_to) == 0)
            {
                repeat = true;
                break;
            }
        }
        if(!repeat)
            out.push_back(*it_from);
    }
}

void StringTool::unique(VecStr& strList)
{
    VecStr result;
    StringTool::unique(strList, result);
    if(strList.size() != result.size())
        strList.assign(result.begin(), result.end());
}

void StringTool::split(const std::string& str, std::string::value_type separator,VecStr& resultOUT,size_t max_out_size)
{
    resultOUT.clear();
    if(max_out_size == 0) 
        return;
    resultOUT.push_back(std::string());
    for(std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if(*it == separator)
        {
            if(!resultOUT.back().empty())
            {
                if(resultOUT.size() == max_out_size) 
                    return;
                resultOUT.push_back(std::string());
            }
        }
        else
        {
            resultOUT.back().push_back(*it);
        }
    }
    if(resultOUT.back().empty())
        resultOUT.pop_back();
}

void StringTool::split(const std::string& src_str, const std::string& separator, VecStr& out, size_t max_out_size)
{
    out.clear();
    if(max_out_size == 0) 
        return;
    if(separator.empty())
    {
        out.push_back(src_str);
        return;
    }
    std::string::size_type begin = 0, index;
    for(;;)
    {
        index = src_str.find(separator, begin);
        if(index == std::string::npos)
            break;
        size_t len = index - begin;
        if(len != 0)
        {
            out.push_back(src_str.substr(begin, len));
            if(out.size() == max_out_size)
                return;
        }
        begin = index + separator.size();
    }
    if(begin < src_str.size() && out.size() < max_out_size)
        out.push_back(src_str.substr(begin));
}

void StringTool::split(const std::string& str, const VecStr& separatorList, VecStr& out, size_t max_out_size)
{
    typedef VecStr container;

    out.clear();
    if(max_out_size == 0) 
        return;
    VecStr splitSeparatorList;
    StringTool::unique(separatorList, splitSeparatorList);
    if(separatorList.empty())
    {
        out.push_back(str);
        return;
    }

    std::string::size_type begin = 0;
    for(std::string::size_type i = 0; i < str.size(); i++)
    {
        for(container::const_iterator it = splitSeparatorList.begin(); it != splitSeparatorList.end(); ++it)
        {
            if(it->compare(0, it->size(), str, i, it->size()) == 0)
            {
                if(i != begin)
                {
                    out.push_back(str.substr(begin, i - begin));
                    if(out.size() == max_out_size)
                        return;
                }
                begin = i + it->size();
                i = begin - 1;
                break;
            }
        }
    }
    if(begin < str.size() && out.size() < max_out_size)
        out.push_back(str.substr(begin));
}

void StringTool::split(const char* str, std::string::value_type separator, VecStr& out, size_t max_out_size)
{
    std::string temp = str;
    StringTool::split(temp, separator, out, max_out_size);
}

void StringTool::replace( std::string& str, const std::string& old_value, const std::string& new_value )
{
    if (old_value.empty())
    {
        return;
    }
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   
    {     
        if(   (pos=str.find(old_value,pos))!=string::npos   )     
            str.replace(pos,old_value.length(),new_value);     
        else   break;     
    }        
}

void StringTool::replace( std::string& str, char old_value, char new_value )
{
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=1)   
    {     
        if(   (pos=str.find(old_value,pos))!=string::npos   )     
            str.replace(pos, 1, 1 ,new_value);     
        else   break;     
    }   
}

const char * StringTool::NumToStr( int num )
{
    static char buf[100];
    snprintf(buf, sizeof(buf), "%d", num);
    return buf;
}

void StringTool::tolower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

