#include "common.h"
#include "engine.h"
#include "order.h"

#ifndef MAIN_C
#define MAIN_C

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
			// 2 types of modify, depends on buy/sell of new order..
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
