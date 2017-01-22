/*
#include <map>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <unordered_map>
*/

#include "engine.h"

using namespace std;

//class CMatchEngine
//{
//public:
	// match a new order in buybook or sellbook, most important function..
void CMatchEngine::Match(COrder& order)
{
	if ( order.GetPrice() == 0 || order.GetQuant() == 0 || order.GetID().empty() )	return;

	if( order.GetOperationType() == ORDER_OPERATION_SELL )
	{ 
			// search buy book..
		list<COrder>::iterator it = m_buybook.begin();
		list<COrder>::iterator ittmp = m_buybook.begin();
		for (; it != m_buybook.end();)
		{
				// sell could happen..
			if (order.GetPrice() <= it->GetPrice() )
			{
				auto orderQty = order.GetQuant();
				auto buyOrderQty = it->GetQuant();
				if (orderQty == buyOrderQty) 
				{
					// delete both
					TradePrint(*it, order, orderQty);
					m_hashbuy.erase(it->GetID());
					ittmp = it;
					it++;
					m_buybook.erase(ittmp);
					return;
				}
				else if(orderQty < buyOrderQty)
				{
					// update book, delete new order
					TradePrint(*it, order, orderQty);
					auto newquant = buyOrderQty - orderQty;
					it->UpdateQuant(newquant);
					return;
				}
				else 
				{
						// update new order.. delete book..
					TradePrint(*it, order, buyOrderQty);
					auto newquant = orderQty - buyOrderQty;
					order.UpdateQuant(newquant);
						
					m_hashbuy.erase(it->GetID());
					ittmp = it;
					it++;
					m_buybook.erase(ittmp);						
					continue;
				}
			}
			it++;
		}

		// insert GFD order into sellbook and hashsell..
		if (order.GetOrderType() == ORDER_TYPE_GFD ) 
		{
			auto tmpitr = m_sellbook.insert(m_sellbook.end(), order);
			m_hashsell[order.GetID()] = tmpitr;
		}
	}

	if (order.GetOperationType() == ORDER_OPERATION_BUY)
	{
		// search SELL book..
		list<COrder>::iterator it = m_sellbook.begin();
		list<COrder>::iterator ittmp = m_sellbook.begin();
		for (;it!=m_sellbook.end();)
		{
			// sell could happen..
			if (order.GetPrice() >= it->GetPrice())
			{
				auto orderQty = order.GetQuant();
				auto sellOrderQty = it->GetQuant();
				if (orderQty == sellOrderQty)
				{
					// delete both
					TradePrint(*it, order, orderQty);

					m_hashsell.erase(it->GetID());
					ittmp = it;
					it++;
					m_sellbook.erase(ittmp);
					return;
				}
				else if (orderQty < sellOrderQty)
				{
					// delete new order.. update book..
					TradePrint(*it, order, orderQty);
					auto newquant = sellOrderQty - orderQty;
					it->UpdateQuant(newquant);
					return;
				}
				else
				{
					// delete book.. update new order..
					TradePrint(*it, order, sellOrderQty);
					auto newquant = orderQty - sellOrderQty;
					order.UpdateQuant(newquant);

					m_hashsell.erase(it->GetID());
					ittmp = it;
					it++;
					m_sellbook.erase(ittmp);
					continue;
				}
			}
			it++;
		}
			
		// insert GFD order into buybook and hashbuy..
		if (order.GetOrderType() == ORDER_TYPE_GFD )
		{
			auto tmpitr = m_buybook.insert(m_buybook.end(), order);
			m_hashbuy[order.GetID()] = tmpitr;
		}
	}
}

void CMatchEngine::Modify(COrder& order)
{
	// firstly try to cancel this order, if cannot be cancelled, means this order do not exist..
	if( Cancel( order ) ) // this order could be cancelled..
	{
		auto operationType =  ORDER_OPERATION_BUY; 
		if( order.GetOperationType() == ORDER_OPERATION_MODIFY_SELL )
		{
			operationType =  ORDER_OPERATION_SELL; 
		}
			
		order.UpdateOperationType( operationType );
		Match( order );
	}
		
	return;
}

// return true if a order was cancelled, return false if no order cancelled..
// triggered by Modify()..
bool CMatchEngine::Cancel(COrder& order)
{
	auto id = order.GetID();
		
	// id could be found in sellbook..
	if( m_hashsell.find(id) != m_hashsell.end() )
	{
		m_sellbook.erase( m_hashsell[ id ] );
		m_hashsell.erase( id );
		return true; // if id could be found in sell book, then wont be found in buy book..
	}
		
	// id could be found in buybook..
	if( m_hashbuy.find(id) != m_hashbuy.end() )
	{
		m_buybook.erase( m_hashbuy[ id ] );
		m_hashbuy.erase( id );
		return true;
	}

	return false;
}
    
void CMatchEngine::Print()
{
	map<uint64_t, uint64_t> sortedBook;
		
	cout << "SELL:" << endl;
	SortBook(sortedBook, m_sellbook);
	for (map<uint64_t, uint64_t>::reverse_iterator rit = sortedBook.rbegin(); rit != sortedBook.rend(); rit++)
	{
		cout << rit->first << " " << rit->second << endl;
	}
		
	cout << "BUY:" << endl;
	SortBook(sortedBook, m_buybook);
	for (map<uint64_t, uint64_t>::reverse_iterator rit = sortedBook.rbegin(); rit != sortedBook.rend(); rit++)
	{
		cout << rit->first << " " << rit->second << endl;
	}
}
	
// sort sellbook and buybook for PRINT operation..
void CMatchEngine::SortBook(map<uint64_t, uint64_t>& sortedBook, list<COrder> orderList)
{
	sortedBook.clear();
	for (auto o : orderList)
	{
		auto price = o.GetPrice();
		if (sortedBook.find(price) == sortedBook.end())
		{
			sortedBook[price] = o.GetQuant();
		}
		else
		{
			auto newPrice = sortedBook[price] + o.GetQuant();
			sortedBook[price] = newPrice;
		}
	}
}

// only used when a trade was done..
void CMatchEngine::TradePrint(COrder& o1, COrder& o2, uint64_t qty)
{
	cout << "TRADE";
	cout << " " << o1.GetID();
	cout << " " << o1.GetPrice();
	cout << " " << qty;
	cout << " " << o2.GetID();
	cout << " " << o2.GetPrice();
	cout << " " << qty;
	cout << endl;
}
// end of class engine..



// parse a cmd line in to a vector of different fields, and return a order object for engine..
// another very important function
COrder ParserCommand(string cmd)
{
	
	vector<string> items;
	SplitString(cmd, " ", items);

	COrder noneOrder(ORDER_OPERATION_NONE);

	if ( items.size() == 0 || items.size() > 5 )	return noneOrder;
    
	ORDER_OPERATION oper = OrderOperationParser(items[0]);
	
    if (oper == ORDER_OPERATION_BUY || oper == ORDER_OPERATION_SELL)
	{
		// check line size, if price and quantity are integer..
		if ( items.size() < 5 || !IsInteger(items[2]) || !IsInteger(items[3]) )	
			return noneOrder;

		COrder newOrder(oper
			, OrderTypeParser(items[1])
			, String2Uint64(items[2])
			, String2Uint64(items[3])
			, items[4]);

		return newOrder;
	}
	else if (oper == ORDER_OPERATION_MODIFY)
	{
		// check line size, if price and quantity are integer..
		if ( items.size() < 5 || !IsInteger(items[4]) || !IsInteger(items[3]) )	
			return noneOrder;
		
		ORDER_OPERATION oper2 = OrderOperationParser(items[2]);
		
        if (oper2 == ORDER_OPERATION_BUY) 
        {
			oper2 = ORDER_OPERATION_MODIFY_BUY;
		}
		else if (oper2 == ORDER_OPERATION_SELL)
		{
			oper2 = ORDER_OPERATION_MODIFY_SELL;
		}
		else 
        {
			return noneOrder;
			//oper2 = ORDER_OPERATION_NONE;
		}

		COrder newOrder(oper2
			, ORDER_TYPE_GFD
			, String2Uint64(items[3])
			, String2Uint64(items[4])
			, items[1]);

		return newOrder;
	}
	else if (oper == ORDER_OPERATION_CANCEL)
	{
		if ( items.size() != 2 )	return noneOrder;

		COrder newOrder(ORDER_OPERATION_CANCEL, ORDER_TYPE_NONE, 0, 0, items[1]);
		return newOrder;
	}
	else if (oper == ORDER_OPERATION_PRINT)
	{
		if ( items.size() != 1 )	return noneOrder;

		COrder newOrder(ORDER_OPERATION_PRINT);
		return newOrder;
	}
	else // ORDER_OPERATION_PRINT..
	{
		return noneOrder;
	}
}
