#include "common.h"

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
