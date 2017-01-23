#include "common.h"

#ifndef ORDER_H
#define ORDER_H

using namespace std;

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

#endif
