///*|-----------------------------------------------------------------------------
// *|            This source code is provided under the Apache 2.0 license      --
// *|  and is provided AS IS with no warranty or guarantee of fit for purpose.  --
// *|                See the project's LICENSE.md for details.                  --
// *|           Copyright Thomson Reuters 2015. All rights reserved.            --
///*|-----------------------------------------------------------------------------

#ifndef __ema_consumer_h_
#define __ema_consumer_h_

#include <iostream>
#include <map>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "Ema.h"
#include "OrderBook.h"
#include "MBPEntry.h"
#include "MBOEntry.h"
#include "OBSummary.h"

void sleep( int millisecs )
{
#if defined WIN32
	::Sleep( (DWORD)(millisecs) );
#else
	struct timespec sleeptime;
	sleeptime.tv_sec = millisecs / 1000;
	sleeptime.tv_nsec = (millisecs % 1000) * 1000000;
	nanosleep( &sleeptime, 0 );
#endif
}


// application defined client class for receiving and processing of item messages
class AppClient : public thomsonreuters::ema::access::OmmConsumerClient	
{

public :
	void dumpSorted();

protected :

	void decodeSummary(const thomsonreuters::ema::access::FieldList&, OBSummary &sd);		// print content of passed in FieldList to screen
	bool decodeMBOEntry(const thomsonreuters::ema::access::FieldList&, MBOEntry &mbo);		// print content of passed in FieldList to screen
	bool decodeMBPEntry(const thomsonreuters::ema::access::FieldList&, MBPEntry &mbp, thomsonreuters::ema::access::MapEntry::MapAction action);		// print content of passed in FieldList to screen
	void decode(const thomsonreuters::ema::access::Map&, thomsonreuters::ema::access::UInt16 domainType);				// print content of passed in Map to screen

	OrderBook<MBOEntry> mboCache;	// Cache the MBO data
	OrderBook<MBPEntry> mbpCache;	// Cache the MBP data

	void onRefreshMsg( const thomsonreuters::ema::access::RefreshMsg&, const thomsonreuters::ema::access::OmmConsumerEvent& );

	void onUpdateMsg( const thomsonreuters::ema::access::UpdateMsg&, const thomsonreuters::ema::access::OmmConsumerEvent& );

	void onStatusMsg( const thomsonreuters::ema::access::StatusMsg&, const thomsonreuters::ema::access::OmmConsumerEvent& );
};

#endif // __ema_consumer_h_
