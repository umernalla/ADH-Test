#pragma once
#include "Ema.h"
#include <string>
#include <sstream>

class OBSummary
{
public:

	OBSummary()
	{
	}

	virtual ~OBSummary()
	{
	}

	std::string displayName;	// Text name of instrument
	std::string timActMs;		// Last activity time to any order 
	std::string activDate;		// Date associated with above time
	thomsonreuters::ema::access::UInt16 exchID; // Source Exchange for the Order Book 
	thomsonreuters::ema::access::UInt16 currency;	// Currency in which the instrument is traded
	thomsonreuters::ema::access::UInt16 prRnkRul;	// Numeric Value indicating how to display the Orders
	thomsonreuters::ema::access::UInt16 orRnkRul;	// Numeric Value indicating how to rank/sort the Orders
	thomsonreuters::ema::access::UInt16 AdDay;		// Activity Date Day
	thomsonreuters::ema::access::UInt16 AdMonth;	// Activity Date Month
	thomsonreuters::ema::access::UInt16 AdYear;		// Activity Date Year

	std::string toString()
	{
		std::ostringstream strStream;
		std::string str;

		strStream << "Exchange ID:" << exchID;
		strStream << " Currency:" << currency;
		strStream << " PrRnkRul:" << prRnkRul;
		strStream << " OrRnkRul:" << orRnkRul;
		strStream << " TimActMS:" << timActMs;
		strStream << " ActiveDate:" << AdDay;
		strStream << "/" << AdMonth ;
		strStream << "/" << AdYear;

		str = strStream.str();

		return str;
	}


};

