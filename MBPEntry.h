#pragma once
#include "OBEntry.h"
#include <string>
#include <sstream>

struct MBPEntry :
	public OBEntry
{
public:

	MBPEntry()
	{
	}

	virtual ~MBPEntry()
	{
	}

	thomsonreuters::ema::access::UInt64 NoOrd;		// Number of orders in MBP row
	double AccSize;									// Total shares represented in MBP row
	thomsonreuters::ema::access::UInt64 LvTimMs;	// Time of most recent  update for MBP Row
	thomsonreuters::ema::access::UInt16 LvDay;		// Day associated with above time
	thomsonreuters::ema::access::UInt16 LvMonth;	// Month associated with above time
	thomsonreuters::ema::access::UInt16 LvYear;		// Year associated with above time

	struct PTS_ascendingPrice
	{
		bool operator()(MBPEntry l, MBPEntry r)
		{
			if (l.OrderPrc < r.OrderPrc) return true;
			if (l.OrderPrc > r.OrderPrc) return false;
			if (l.PrTimMs < r.PrTimMs) return true;
			if (l.PrTimMs > r.PrTimMs) return false;
			return (l.AccSize < r.AccSize);
		}
	};

	struct PTS_descendingPrice
	{
		bool operator()(MBPEntry l, MBPEntry r)
		{
			if (l.OrderPrc > r.OrderPrc) return true;
			if (l.OrderPrc < r.OrderPrc) return false;
			if (l.PrTimMs < r.PrTimMs) return true;
			if (l.PrTimMs > r.PrTimMs) return false;
			return (l.AccSize < r.AccSize);
		}
	};

	struct PST_ascendingPrice
	{
		bool operator()(MBPEntry l, MBPEntry r)
		{
			if (l.OrderPrc < r.OrderPrc) return true;
			if (l.OrderPrc > r.OrderPrc) return false;
			if (l.AccSize < r.AccSize) return true;
			if (l.AccSize > r.AccSize) return false;
			return (l.PrTimMs < r.PrTimMs);
		}
	};

	struct PST_descendingPrice
	{
		bool operator()(MBPEntry l, MBPEntry r)
		{
			if (l.OrderPrc > r.OrderPrc) return true;
			if (l.OrderPrc < r.OrderPrc) return false;
			if (l.AccSize < r.AccSize) return true;
			if (l.AccSize > r.AccSize) return false;
			return (l.PrTimMs < r.PrTimMs);
		}
	};


	std::string toString() const
	{
		std::ostringstream strStream;
		std::string str;
		str = OBEntry::toString();
		strStream	<< AccSize << " : "
					<< NoOrd << " : "
					<< LvDay << "/" << LvMonth << "/" << LvYear;
		str += strStream.str();
		return str;
	}

};

