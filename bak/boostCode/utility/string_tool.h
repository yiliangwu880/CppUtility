#pragma once

#include <string>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>


/*
use example:
string s = "a b,cd,43;";
StrSplit ss(s);
BOOST_FOREACH( string sub_str, ss )
{
	count << sub_str << endl;
}
*/
typedef boost::tokenizer<> StrSplit;

