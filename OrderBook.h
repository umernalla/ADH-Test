#ifndef __orderbook_h_
#define __orderbook_h_

#include "Ema.h"
#include "OBSummary.h"
#include <vector>

template <class T>
class OrderBook
{
public:

	typedef std::map<std::string, T > OrderBookEntries;

	OrderBook()
	{
	}

	virtual ~OrderBook()
	{
	}

	std::string RicCode;	// Symbol for the OrderBook

	OBSummary summaryData;	// Summary Data for the OrderBook

	OrderBookEntries entries;	// Map of OrderBook Entries / rows

	void outputBook()
	{
		typename OrderBook<T>::OrderBookEntries::const_iterator it = entries.begin();
		cout << "SUMMARY DATA : " ;
		std::cout << summaryData.toString() << endl;
		while (it != entries.end())
		{
			std::cout << "KEY:" << it->first;
			std::cout << " ENTRY:" << it->second.toString() << endl;
			it++;
		}
	}
	
	std::vector<T> asVector()
	{
		std::vector<T> obVector;
		typename OrderBook<T>::OrderBookEntries::const_iterator it = entries.begin();
		while (it != entries.end())
		{
			obVector.push_back(it->second);
			it++;
		}
		return obVector;
	}

	void asVector(std::vector<T> & obVector )
	{
		typename OrderBook<T>::OrderBookEntries::const_iterator it = entries.begin();
		while (it != entries.end())
		{
			obVector.push_back(it->second);
			it++;
		}
	}

	void asBidAskVectors(std::vector<T> & bidVectors, std::vector<T> & askVectors )
	{
		typename OrderBook<T>::OrderBookEntries::const_iterator it = entries.begin();
		thomsonreuters::ema::access::UInt16 orderSide;	// Order Side : 1=BID, 2=ASK
		while (it != entries.end())
		{
			orderSide = it->second.OrderSide;
			switch (orderSide)
			{
				case fids::OrderSide_BID:
					bidVectors.push_back(it->second);
					break;
				case fids::OrderSide_ASK:
					askVectors.push_back(it->second);
					break;
			}
			it++;
		}
	}

};

#endif