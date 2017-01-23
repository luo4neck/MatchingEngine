//#include <map>
//#include <list>
//#include <string>
#include <vector>
#include <fstream>
//#include <numeric>
#include <sstream>
//#include <iostream>
//#include <unordered_map>

//#include "order.hpp"

#include "common.h"

//#ifndef COMMON
//#define COMMON

//using namespace std;

/*
// order types..
enum ORDER_TYPE 
{
	ORDER_TYPE_NONE,
	ORDER_TYPE_GFD,
	ORDER_TYPE_IOC
};
*/

// order type parser..
ORDER_TYPE OrderTypeParser(const string& orderType)
{
	if ("GFD" == orderType) 
    {
		return ORDER_TYPE_GFD;
	}
	if ("IOC" == orderType) 
    {
		return ORDER_TYPE_IOC;
	}
	return ORDER_TYPE_NONE;
}


/*
// operation types.. 
enum ORDER_OPERATION 
{
	ORDER_OPERATION_NONE,
	ORDER_OPERATION_BUY,
	ORDER_OPERATION_SELL,
	ORDER_OPERATION_MODIFY,
	ORDER_OPERATION_MODIFY_BUY,
	ORDER_OPERATION_MODIFY_SELL,
	ORDER_OPERATION_CANCEL,
	ORDER_OPERATION_PRINT
};
*/

// operation type parser..
ORDER_OPERATION OrderOperationParser(const string& orderType)
{
	if ("BUY" == orderType) 
    {
		return ORDER_OPERATION_BUY;
	}
	if ("SELL" == orderType) 
    {
		return ORDER_OPERATION_SELL;
	}
	if ("MODIFY" == orderType) 
    {
		return ORDER_OPERATION_MODIFY;
	}
	if ("CANCEL" == orderType) 
    {
		return ORDER_OPERATION_CANCEL;
	}
	if ("PRINT" == orderType) 
    {
		return ORDER_OPERATION_PRINT;
	}
	return ORDER_OPERATION_NONE;
}

/*
// used in parser, split a cmd line separated by ' ' into a vector of string..
inline void SplitString(const string& s, const string& delim, vector<string>& ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != string::npos)
	{
		string newitem = s.substr(last, index - last);
		if (newitem.length() > 0)
		{
			ret.push_back(s.substr(last, index - last));
		}
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if ( index - last > 0 )
	{
		string newitem = s.substr(last, index - last);
		if (newitem.length() > 0)
		{
			ret.push_back(newitem);
		}
	}
}

inline uint64_t String2Uint64(const string& s)
{
	stringstream a;
	a << s;
	uint64_t ret = 0;
	a >> ret; return ret;
}

inline bool IsInteger(const string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

*/
//#endif
