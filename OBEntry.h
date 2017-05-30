#pragma once
#include "Ema.h"
#include <string>

struct OBEntry
{
public:

	OBEntry()
	{
	}

	virtual  ~OBEntry()
	{
	}


	double OrderPrc;	// Order Price
	thomsonreuters::ema::access::UInt16 OrderSide;	// Order Side : 1=BID, 2=ASK
	char OrderSideChar;								// Char representation B / A
	thomsonreuters::ema::access::UInt64 PrTimMs;	// Priority Time used for Ranking


	std::string toString() const
	{
		std::ostringstream strStream;
		std::string str;
		strStream	<< OrderSideChar << " : "
					<< OrderPrc << " : "
					<< PrTimMs << " : ";
		str = strStream.str();
		return str;
	}


};

