#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <limits.h>
#include <errno.h>
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include "cnt_typedef.h"


class StringTool
{
    
public:
	static const std::string WHITE_SPACE;
	static const int DEFAULT_BUFFER_SIZE = 1024;
public:
	static std::string& format(std::string& resultOUT, const char* fmt, ...);
	static std::string format(const char* fmt, ...);
	static std::string& vformat(std::string& resultOUT, const char *fmt, va_list argptr, int default_size = DEFAULT_BUFFER_SIZE);
	static std::string vformat(const char *fmt, va_list argptr, int default_size = DEFAULT_BUFFER_SIZE);

	static bool format_index(std::string& resultOUT, const char* fmt, const VecStr& argList);
	static bool format_index(std::string& resultOUT, const char* fmt);
	template<typename Arg0>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0);
	template<typename Arg0, typename Arg1>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1);
	template<typename Arg0, typename Arg1, typename Arg2>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2);
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3);
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4);
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5);
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6);
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	static bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7);

	static std::string& tostring(std::string& resultOUT, char c);
	static std::string& tostring(std::string& resultOUT, const char* pstr);
	static std::string& tostring(std::string& resultOUT, const std::string& str);
	static std::string& tostring(std::string& resultOUT, bool value);
	template<typename ValueType>
	static std::string& tostring(std::string& resultOUT, ValueType value);
	//string to num 可以用atoi atoll等

	static std::string& append(std::string& str, const char* fmt, ...);
	static std::string& append(std::string& str, const std::string& cat);
	static std::string& append(std::string& str, char c);
	static std::string& append(std::string& str, bool value);
	template<typename ValueType>
	static std::string& append(std::string& str, ValueType value);

	static std::string trimbegin(std::string& str, const std::string& whiteSpace = WHITE_SPACE);
	static std::string trimend(std::string& str, const std::string& whiteSpace = WHITE_SPACE);
	static std::string trim(std::string& str, const std::string& whiteSpace = WHITE_SPACE);

	static bool contains(const std::string& str, const std::string& tosearch);
	static bool contains(const std::string& str, const char* tosearch);
	static bool startwith(const std::string& str, const std::string& needle);
	static bool endwith(const std::string& str, const std::string& needle);

	static std::string& padleft(std::string& str, std::string::size_type size, std::string::value_type paddingchar = ' ');
	static std::string& padright(std::string& str, std::string::size_type size, std::string::value_type paddingchar = ' ');

	static void unique(const VecStr& strList, VecStr& out);

	static void unique(VecStr& strList);

	static bool try_parse_int(const char* str, int* ret, char** end = NULL);

	static void split(const std::string& str, std::string::value_type separator, VecStr& out, size_t max_out_size = 2000);
	static void split(const std::string& str, const std::string& separator, VecStr& out, size_t max_out_size = 2000);

	static void split(const std::string& str, const VecStr& separatorList, VecStr& out, size_t max_out_size = 2000);
	static void split(const char* str, std::string::value_type separator, VecStr& out, size_t max_out_size = 2000);
	static void split_white_space(const std::string& str, VecStr& out, size_t resultLimit = (size_t)-1);
	static bool split_fix_size(const std::string& str, std::string::size_type fix_size, VecStr& out);

	template <class InputIterator>
	static std::string& join(const std::string &glue, InputIterator first, InputIterator last, std::string& out);
	template <class InputIterator>
	static std::string join(const std::string &glue, InputIterator first, InputIterator last);

	static void replace(std::string& str, const std::string& from, const std::string& to);
	static void replace(std::string& str, char from, char to);

    static void erase(std::string& str, const std::string& to_remove);
    static void erase(std::string& str, char c);

	static long int integer(const std::string& str);
	static bool integer(const std::string& str, long int& valueOut);

    //不支持多线程
    static const char *NumToStr(int num);

	static std::string& repeat(const std::string& str, std::string::size_type times, std::string& resultOUT);
	static std::string repeat(const std::string& str, std::string::size_type times);

	static bool is_lower(const std::string& str);
	static bool is_lower(char c);
	static bool is_upper(const std::string& str);
	static bool is_upper(char c);
	static bool is_alpha(const std::string& str);
	static bool is_alpha(char c);
	static bool is_numeric(const std::string& str);
	static bool is_numeric(char c);
	static bool is_alnum(const std::string& str);
	static bool is_alnum(char c);
	static bool is_whitespace(char c);

	static void tolower(std::string& str);
	static char* tolower(char* pstr);
	static char tolower(char c);
	static std::string& toupper(std::string& str);
	static char toupper(char c);
	static std::string& swapcase(std::string& str);
	static char swapcase(char c);

	static std::string::size_type count(const std::string& str, const std::string &module);
	static int compare(const char* str1, const char* str2);
	static int compare(const std::string& str1, const char* str2);
	static int compare(const std::string& str1, const std::string& str2);
	static int compare(const char* str1, const std::string& str2);
	static int compare(char c1, char c2);

	static int icompare(const std::string& str1, const std::string& str2);
	static int icompare(const char* str1, const char* str2);
	static int icompare(char c1, char c2);

	static int compare_nature(const char *str1, const char *str2, bool case_sensitive = true);
	static int compare_nature(const std::string& str1, const std::string& str2, bool case_sensitive = true);

	static bool match_wildcard(const char* str, const char* wildcard);

	static bool empty(const char* pstr);
	static bool empty(const std::string& str);
	static bool empty(char c);

	static size_t strlen(const char* pstr);
	static size_t strlen(const std::string& str);

	static bool hasPrefix(const std::string& str, const std::string& prefix);
	static std::string stripPrefix(const std::string& str, const std::string& prefix);
	static bool hasSuffix(const std::string& str, const std::string& suffix);
	static std::string stripSuffix(const std::string& str, const std::string& suffix);

	template<typename CharType>	
	static int hash_code(const CharType* str);

	static int LevenshteinDistance(const std::string& source, const std::string& target);

	template<typename T>
	static std::string NumToStr(T num)
	{
		std::stringstream ss;
		ss << num;

		return ss.str();
	}
private:
	StringTool(){}
};

inline std::string StringTool::vformat(const char *fmt, va_list argptr, int default_size)
{
	std::string resultOUT;
	return vformat(resultOUT, fmt, argptr, default_size);
}
inline bool StringTool::format_index(std::string& resultOUT, const char* fmt)
{
	return format_index(resultOUT, fmt, VecStr());
}
template<typename Arg0>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0)
{
	VecStr args(1);
	tostring(args[0], arg0);
	return format_index(resultOUT, fmt, args);
}
template<typename Arg0, typename Arg1>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1)
{
	VecStr args(2);
	tostring(args[0], arg0); tostring(args[1], arg1);
	return format_index(resultOUT, fmt, args);
}
template<typename Arg0, typename Arg1, typename Arg2>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2)
{
	VecStr args(3);
	tostring(args[0], arg0); tostring(args[1], arg1); tostring(args[2], arg2);
	return format_index(resultOUT, fmt, args);
}
template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
	VecStr args(4);
	tostring(args[0], arg0); tostring(args[1], arg1); tostring(args[2], arg2); tostring(args[3], arg3); 
	return format_index(resultOUT, fmt, args);
}
template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
	VecStr args(5);
	tostring(args[0], arg0); tostring(args[1], arg1); tostring(args[2], arg2); tostring(args[3], arg3); tostring(args[4], arg4);
	return format_index(resultOUT, fmt, args);
}
template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
{
	VecStr args(6);
	tostring(args[0], arg0); tostring(args[1], arg1); tostring(args[2], arg2); tostring(args[3], arg3); tostring(args[4], arg4); tostring(args[5], arg5);
	return format_index(resultOUT, fmt, args);
}
template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
{
	VecStr args(7);
	tostring(args[0], arg0); tostring(args[1], arg1); tostring(args[2], arg2); tostring(args[3], arg3); tostring(args[4], arg4); tostring(args[5], arg5); tostring(args[6], arg6);
	return format_index(resultOUT, fmt, args);
}
template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
bool StringTool::format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7)
{
	VecStr args(8);
	tostring(args[0], arg0); tostring(args[1], arg1); tostring(args[2], arg2); tostring(args[3], arg3); tostring(args[4], arg4); tostring(args[5], arg5); tostring(args[6], arg6); tostring(args[7], arg7);
	return format_index(resultOUT, fmt, args);
}

inline std::string& StringTool::tostring(std::string& resultOUT, char c)
{
	resultOUT = c;
	return resultOUT;
}
inline std::string& StringTool::tostring(std::string& resultOUT, const char* pstr)
{
	resultOUT = pstr == NULL ? "" : pstr; 
	return resultOUT;
}
inline std::string& StringTool::tostring(std::string& resultOUT, const std::string& str)
{
	resultOUT = str; 
	return resultOUT;
}
inline std::string& StringTool::tostring(std::string& resultOUT, bool value)
{
	resultOUT = value ? "true" : "false"; 
	return resultOUT;
}
template<typename ValueType>
inline std::string& StringTool::tostring(std::string& resultOUT, ValueType value)
{
	std::stringstream oss;
	oss << value;
	resultOUT = oss.str();
	return resultOUT;
} 
inline std::string& StringTool::append(std::string& str, const std::string& cat)
{
	str += cat;
	return str;
}
inline std::string& StringTool::append(std::string& str, char c)
{
	str += c;
	return str;
}
inline std::string& StringTool::append(std::string& str, bool value)
{
	str += value ? "true" : "false";
	return str;
}
template<typename ValueType>
inline std::string& StringTool::append(std::string& str, ValueType value)
{
	std::stringstream oss;
	oss << value;
	str += oss.str();
	return str;
}
inline std::string StringTool::trimbegin(std::string& str, const std::string& whiteSpace)
{
	return str.erase(0,str.find_first_not_of(whiteSpace));
}
inline std::string StringTool::trimend(std::string& str, const std::string& whiteSpace)
{
	return str.erase(str.find_last_not_of(whiteSpace)+1);
}
inline std::string StringTool::trim(std::string& str, const std::string& whiteSpace)
{
	str.erase(str.find_last_not_of(whiteSpace)+1);
	return str.erase(0,str.find_first_not_of(whiteSpace));
}
inline bool StringTool::contains(const std::string& str, const std::string& tosearch)
{
	return str.find(tosearch) != std::string::npos;
}
inline bool StringTool::contains(const std::string& str, const char* tosearch)
{
	if(tosearch == NULL)
		return false;
	return str.find(tosearch) != std::string::npos;
}
inline bool StringTool::startwith(const std::string& str, const std::string& needle)
{
	if(str.size() < needle.size())
		return false;
	return str.compare(0, needle.size(), needle) == 0;
}
inline bool StringTool::endwith(const std::string& str, const std::string& needle)
{
	if(str.size() < needle.size())
		return false;
	return str.compare(str.size() - needle.size(), needle.size(), needle) == 0;
}
inline std::string& StringTool::padleft(std::string& str, std::string::size_type size, std::string::value_type paddingchar)
{
	if(size > str.size())
		str.insert(0, size - str.size(), paddingchar);
	return str;
}
inline std::string& StringTool::padright(std::string& str, std::string::size_type size, std::string::value_type paddingchar)
{
	if(size > str.size())
		str.resize(size, paddingchar);
	return str;
}

template <class InputIterator>
inline std::string& StringTool::join(const std::string &glue, InputIterator first, InputIterator last, std::string& resultOUT)
{
	resultOUT.clear();
	if(first == last)
		return resultOUT;
	for(; first != last; first++)
	{
		resultOUT.append(*first);
		resultOUT.append(glue);
	}
	resultOUT.erase(resultOUT.size() - glue.size());
	return resultOUT;
}
template <class InputIterator>
inline std::string StringTool::join(const std::string &glue, InputIterator first, InputIterator last)
{
	std::string resultOUT;
	return join(glue, first, last, resultOUT);
}
inline void StringTool::erase(std::string& str, const std::string& to_remove)
{
	replace(str, to_remove, "");
}

inline void StringTool::erase(std::string& str, char c)
{
    std::string s(1,c);
    replace(str, s, "");
}


inline std::string StringTool::repeat(const std::string& str, std::string::size_type times)
{
	std::string resultOUT;
	return repeat(str, times, resultOUT);
}
inline bool StringTool::is_lower(char c)
{
	return c >= 'a' && c <= 'z';
}
inline bool StringTool::is_upper(char c)
{
	return c >= 'A' && c <= 'Z';
}
inline bool StringTool::is_alpha(char c)
{
	return is_lower(c) || is_upper(c);
}
inline bool StringTool::is_numeric(char c)
{
	return c >= '0' && c <= '9';
}
inline bool StringTool::is_alnum(char c)
{
	return is_alpha(c) || is_numeric(c);
}
inline bool StringTool::is_whitespace(char c)
{
	return WHITE_SPACE.find(c) != std::string::npos;
}
inline char StringTool::tolower(char c)
{
	return is_upper(c) ? c - 'A' + 'a' : c; 
}
inline char StringTool::toupper(char c)
{
	return is_lower(c) ? c - 'a' + 'A' : c;
}
inline char StringTool::swapcase(char c)
{
	if(is_upper(c)) 
		return c - 'A' + 'a';
	if(is_lower(c)) 
		return c - 'a' + 'A';
	return c;
}
inline int StringTool::compare(const char* str1, const char* str2)
{
	if(str1 == NULL)
	{
		if(str2 == NULL) 
			return 0; 
		else	
			return -1;
	}
	else
	{
		if(str2 == NULL) 
			return 1; 
		else 
			return strcmp(str1, str2);
	}
}
inline int StringTool::compare(const std::string& str1, const char* str2)
{
	if(str2 == NULL) 
		return 1; 
	else 
		return str1.compare(str2);
}
inline int StringTool::compare(const std::string& str1, const std::string& str2)
{
	return str1.compare(str2);
}
inline int StringTool::compare(const char* str1, const std::string& str2)
{
	return -compare(str2, str1);
}
inline int StringTool::compare(char c1, char c2)
{
	if(c1 > c2)
		return 1;
	else if(c1 < c2)
		return -1;
	else
		return 0;
}
inline int StringTool::icompare(const char* str1, const char* str2)
{
	if(str1 == str2)
		return 0;
	if(str1 == NULL)
		return -1;
	if(str2 == NULL)
		return 1;

	return strcasecmp(str1, str2);

}
inline int StringTool::icompare(char c1, char c2)
{
	if(c1 == c2)
		return 0;
	if(is_alpha(c1))
		c1 = tolower(c1);
	if(is_alpha(c2))
		c2 = tolower(c2);
	if(c1 > c2)
		return 1;
	else
		return -1;
}
inline int StringTool::compare_nature(const std::string& str1, const std::string& str2, bool case_sensitive /* = true*/)
{
	return compare_nature(str1.c_str(), str2.c_str(), case_sensitive);
}

inline bool StringTool::empty(const char* pstr)
{
	return pstr == NULL || pstr[0] == '\0';
}
inline bool StringTool::empty(const std::string& str)
{
	return str.empty();
}
inline bool StringTool::empty(char c)
{
	return c == '\0';
}
inline size_t StringTool::strlen(const char* pstr)
{
	return empty(pstr)? 0 : ::strlen(pstr);
}
inline size_t StringTool::strlen(const std::string& str)
{
	return str.size();
}
inline bool StringTool::try_parse_int(const char* str, int* ret, char** end /* = NULL*/)
{
	long int n = 0;
	char* p = NULL;
	if(str == NULL || ret == NULL)
		goto L_ErrorEnd;	
	n = strtol(str, &p, 0);
	if(n == LONG_MAX || n == LONG_MIN)
	{
		if(errno == ERANGE)
			goto L_ErrorEnd;
	}
	else if(n == 0)
	{
		if(p == str)
			goto L_ErrorEnd;
	}
	if(end != NULL)
		*end = p;
	*ret = n;
	return true;
L_ErrorEnd:
	if(end != NULL)
		*end = const_cast<char*>(str);
	if(ret != NULL)
		*ret = 0;
	return false;
}

template<typename CharType>
inline int StringTool::hash_code(const CharType* str)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	while(*str)
	{
		hash = (hash << 4) + (*str++);
		if((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}
	return (hash & 0x7FFFFFFF);
}
