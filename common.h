//#include <map>
//#include <list>
#include <string>
#include <vector>
#include <fstream>
//#include <numeric>
#include <sstream>
//#include <iostream>
//#include <unordered_map>
//#include "order.hpp"

#ifndef COMMON
#define COMMON

using namespace std;

// order types..
enum ORDER_TYPE 
{
	ORDER_TYPE_NONE,
	ORDER_TYPE_GFD,
	ORDER_TYPE_IOC
};

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

// order type parser..
ORDER_TYPE OrderTypeParser(const string& orderType);

// operation type parser..
ORDER_OPERATION OrderOperationParser(const string& orderType);

// used in parser, split a cmd line separated by ' ' into a vector of string..
void SplitString(const string& s, const string& delim, vector<string>& ret);

inline uint64_t String2Uint64(const string& s);

inline bool IsInteger(const string & s);

#endif
