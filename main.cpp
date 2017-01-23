//#include <map>
//#include <list>
#include <string>
/*
#include <vector>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <unordered_map>
*/
#include "common.h"
#include "engine.h"
#include "order.h"

#ifndef MAIN
#define MAIN

using namespace std;

int main()
{
	string cmdLine;
	CMatchEngine engine;
	while ( getline( cin , cmdLine ) )
	{
		COrder newOrder = CMatchEngine::ParserCommand(cmdLine);

		switch (newOrder.GetOperationType())
		{
			case ORDER_OPERATION_BUY:
			case ORDER_OPERATION_SELL:
				{
					if( newOrder.GetOrderType() == ORDER_TYPE_NONE )
						break;

					engine.Match(newOrder);
				}
				break;
			//case ORDER_OPERATION_MODIFY:
			case ORDER_OPERATION_MODIFY_BUY:
			case ORDER_OPERATION_MODIFY_SELL:
				{
					engine.Modify(newOrder);
				}
				break;
			case ORDER_OPERATION_CANCEL:
				{
					engine.Cancel(newOrder);
				}
				break;
			case ORDER_OPERATION_PRINT:
				{
					engine.Print();
				}
				break;
			default: // ORDER_OPERATION_NONE
				break;
		}
	}

	return 0;
}

#endif
