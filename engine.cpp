#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

// order types..
enum ORDER_TYPE 
{
	ORDER_TYPE_NONE,
	ORDER_TYPE_GFD,
	ORDER_TYPE_IOC
};


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
	void Match(COrder& order)
	{
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
						TradePrint(*it, order, orderQty);
						m_hashbuy.erase(it->GetID());
						it++;
						m_buybook.erase(ittmp);
						return;
					}
					else if(orderQty < buyOrderQty)
					{
						TradePrint(*it, order, orderQty);
						auto newquant = buyOrderQty - orderQty;
						it->UpdateQuant(newquant);
						return;
					}
					else 
					{
						TradePrint(*it, order, buyOrderQty);
						auto newquant = orderQty - buyOrderQty;
						order.UpdateQuant(newquant);
						
						m_hashbuy.erase(it->GetID());
						it++;
						m_buybook.erase(ittmp);						
					}
				}
			}

			if(order.GetOrderType() == ORDER_TYPE_GFD)
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
						TradePrint(*it, order, orderQty);

						m_hashsell.erase(it->GetID());
						it++;
						m_sellbook.erase(ittmp);
						return;
					}
					else if (orderQty < sellOrderQty)
					{
						TradePrint(*it, order, orderQty);
						auto newquant = sellOrderQty - orderQty;
						it->UpdateQuant(newquant);
						return;
					}
					else
					{
						TradePrint(*it, order, sellOrderQty);
						auto newquant = orderQty - sellOrderQty;
						order.UpdateQuant(newquant);

						m_hashsell.erase(it->GetID());
						it++;
						m_sellbook.erase(ittmp);
					}
				}
			}
			if (order.GetOrderType() == ORDER_TYPE_GFD)
			{
				auto tmpitr = m_buybook.insert(m_buybook.end(), order);
				m_hashbuy[order.GetID()] = tmpitr;
			}
		}
	}

	void Modify(COrder& order)
	{
		
	}

	void Cancel(COrder& order)
	{
		auto id = order.GetID();
		
		// id could be found in sellbook..
		if( m_hashsell.find(id) != m_hashsell.end() )
		{
			m_sellbook.erase( m_hashsell[ id ] );
			m_hashsell.erase( id );
			return; // if id could be found in sell book, then wont be found in buy book..
		}
		
		// id could be found in buybook..
		if( m_hashbuy.find(id) != m_hashbuy.end() )
		{
			m_buybook.erase( m_hashbuy[ id ] );
			m_hashbuy.erase( id );
			return;
		}
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

        cout<<endl;
	}
	
    // only used in print..
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

// used in parser..
void SplitString(const string& s, const string& delim, vector< string >& ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != string::npos)
	{
		ret.push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last>0)
	{
		ret.push_back(s.substr(last, index - last));
	}
}

uint64_t String2Uint64(const string& s)
{
	stringstream a;
	a << s;
	uint64_t ret = 0;
	a >> ret;
	return ret;
}

COrder ParserCommand(string cmd)
{
	vector<string> items;
	SplitString(cmd, " ", items);
	if (items.size() == 0)
	{
		COrder newOrder(ORDER_OPERATION_NONE);
		return newOrder;
	}
    
	ORDER_OPERATION oper = OrderOperationParser(items[0]);
	
    if (oper == ORDER_OPERATION_BUY || oper == ORDER_OPERATION_SELL)
	{
		if (items.size() < 5)
		{
			COrder newOrder(ORDER_OPERATION_NONE);
			return newOrder;
		}
		COrder newOrder(oper
			, OrderTypeParser(items[1])
			, String2Uint64(items[2])
			, String2Uint64(items[3])
			, items[4]);
		return newOrder;
	}
	else if (oper == ORDER_OPERATION_MODIFY)
	{
		if (items.size() < 5)
		{
			COrder newOrder(ORDER_OPERATION_NONE);
			return newOrder;
		}
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
			oper2 = ORDER_OPERATION_NONE;
		}

		COrder newOrder(oper2
			, ORDER_TYPE_NONE
			, String2Uint64(items[3])
			, String2Uint64(items[4])
			, items[1]);
		return newOrder;
	}
	else if (oper == ORDER_OPERATION_CANCEL)
	{
		if (items.size() < 2)
		{
			COrder newOrder(ORDER_OPERATION_NONE);
			return newOrder;
		}
		COrder newOrder(ORDER_OPERATION_CANCEL, ORDER_TYPE_NONE, 0, 0, items[1]);
		return newOrder;
	}
	else if (oper == ORDER_OPERATION_PRINT)
	{
		COrder newOrder(ORDER_OPERATION_PRINT);
		return newOrder;
	}
	//none
	else
	{
		COrder newOrder(ORDER_OPERATION_NONE);
		return newOrder;
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

int main()
{
	string cmdLine;
	CMatchEngine engine;
	while (getline(cin,cmdLine))
	{	
		COrder newOrder = ParserCommand(cmdLine);
		switch (newOrder.GetOperationType())
		{
			case ORDER_OPERATION_BUY:
			case ORDER_OPERATION_SELL:
				{
					engine.Match(newOrder);
				}
				break;
			case ORDER_OPERATION_MODIFY:
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
			default:
				// do nothing
				break;
		}
	}

	return 0;
}
