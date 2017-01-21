#include <map>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "common.hpp"
#include "engine.hpp"
#include "order.hpp"

/*
void Test01()
{
	COrder o1(ORDER_OPERATION_BUY, ORDER_TYPE_GFD, 1000, 10, "order1");
	COrder o2(ORDER_OPERATION_SELL, ORDER_TYPE_GFD, 1000, 5, "order2");
	COrder o3(ORDER_OPERATION_SELL, ORDER_TYPE_GFD, 900, 3, "order3");
	CMatchEngine engine;
	engine.Match(o1);
	engine.Match(o2);
	engine.Match(o3);
}
*/
/* test case 2
BUY GFD 1000 10 order1
BUY GFD 1000 10 order2
SELL IOC 900 2 order4
SELL GFD 1200 2 order5
PRINT
MODIFY order1 BUY 1300 20
SELL GFD 900 20 order3
CANCEL order1
PRINT
SELL GFD 2122 20 order6
SELL ioc 213213 20 order7
SELL GFD 12 20 order8
sdfa 215 5212 sdf 5432
SELL IOC 90 20 order9
SELL GFD 00 20 order11
SELL GFD 900 20 order12
BUY IOC 122 FS order13
SELL GFD 900 20 order14
PRINT
*/ 

int main()
{
	string cmdLine;
	CMatchEngine engine;
	while ( getline( cin , cmdLine ) )
	{
		COrder newOrder = ParserCommand(cmdLine);

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
