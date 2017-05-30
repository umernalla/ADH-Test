#pragma once
#include "OBEntry.h"
#include <string>
#include <sstream>

struct MBOEntry :
	public OBEntry
{
public:

	MBOEntry()
	{
	}

	virtual ~MBOEntry()
	{
	}

	thomsonreuters::ema::access::EmaString OrderID;
	double OrderSize;

	struct PTS_ascendingPrice
	{
		bool operator()(MBOEntry l, MBOEntry r)
		{
			if (l.OrderPrc < r.OrderPrc) return true;
			if (l.OrderPrc > r.OrderPrc) return false;
			if (l.PrTimMs < r.PrTimMs) return true;
			if (l.PrTimMs > r.PrTimMs) return false;
			return (l.OrderSize < r.OrderSize);
		}
	};

	struct PTS_descendingPrice
	{
		bool operator()(MBOEntry l, MBOEntry r)
		{
			if (l.OrderPrc > r.OrderPrc) return true;
			if (l.OrderPrc < r.OrderPrc) return false;
			if (l.PrTimMs < r.PrTimMs) return true;
			if (l.PrTimMs > r.PrTimMs) return false;
			return (l.OrderSize < r.OrderSize);
		}
	};

	struct PST_ascendingPrice
	{
		bool operator()(MBOEntry l, MBOEntry r)
		{
			if (l.OrderPrc < r.OrderPrc) return true;
			if (l.OrderPrc > r.OrderPrc) return false;
			if (l.OrderSize < r.OrderSize) return true;
			if (l.OrderSize > r.OrderSize) return false;
			return (l.PrTimMs < r.PrTimMs);
		}
	};

	struct PST_descendingPrice
	{
		bool operator()(MBOEntry l, MBOEntry r)
		{
			if (l.OrderPrc > r.OrderPrc) return true;
			if (l.OrderPrc < r.OrderPrc) return false;
			if (l.OrderSize < r.OrderSize) return true;
			if (l.OrderSize > r.OrderSize) return false;
			return (l.PrTimMs < r.PrTimMs);
		}
	};


	std::string toString() const
	{
		std::ostringstream strStream;
		std::string str;
		str = OBEntry::toString();
		strStream	<< OrderSize ;
		str += strStream.str();
		return str;
	}


};

