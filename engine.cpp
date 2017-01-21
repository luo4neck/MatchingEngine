#include <map>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <unordered_map>

using namespace std;

// order types..
enum ORDER_TYPE 
{
	ORDER_TYPE_NONE,
	ORDER_TYPE_GFD,
	ORDER_TYPE_IOC
};

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


class COrder
{
public:

	COrder(ORDER_OPERATION operationType, ORDER_TYPE orderType, uint64_t price, uint64_t quant, string id) :
	 m_operationType(operationType),
	 m_orderType(orderType),
	 m_price(price),
	 m_quant(quant),
	 m_id(id)
	{}

	// constructor for none order..
	COrder(ORDER_OPERATION operationType) :
		m_operationType(operationType),
		m_orderType(ORDER_TYPE_NONE),
		m_price(0),
		m_quant(0),
		m_id("")
	{}

	ORDER_OPERATION GetOperationType() { return m_operationType; }

	ORDER_TYPE GetOrderType() { return m_orderType; }

	uint64_t GetPrice() { return m_price; }

	uint64_t GetQuant() { return m_quant; }

	string GetID() { return m_id; }

	void UpdateQuant(uint64_t newquant) { m_quant = newquant; }

	void UpdateOperationType(ORDER_OPERATION newtype) { m_operationType = newtype; }

private:
	COrder() {};

private:
	ORDER_OPERATION m_operationType;
	ORDER_TYPE m_orderType;
	uint64_t m_price;
	uint64_t m_quant;
	string m_id;
};

class CMatchEngine
{
public:
	// match a new order in buybook or sellbook, most important function..
	void Match(COrder& order)
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

	void Modify(COrder& order)
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
	bool Cancel(COrder& order)
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
    
	void Print()
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
    void SortBook(map<uint64_t, uint64_t>& sortedBook, list<COrder> orderList)
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

private:
    // only used when a trade was done..
	void TradePrint(COrder& o1, COrder& o2, uint64_t qty)
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
private:
	unordered_map<string, list<COrder>::iterator> m_hashsell;
	unordered_map<string, list<COrder>::iterator> m_hashbuy;
	list<COrder>  m_sellbook;
	list<COrder>   m_buybook;
};

// used in parser, split a cmd line separated by ' ' into a vector of string..
void SplitString(const string& s, const string& delim, vector<string>& ret)
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

// a simple test case..
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
					auto ret = engine.Cancel(newOrder);
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
