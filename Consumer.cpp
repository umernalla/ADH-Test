///*|-----------------------------------------------------------------------------
// *|            This source code is provided under the Apache 2.0 license      --
// *|  and is provided AS IS with no warranty or guarantee of fit for purpose.  --
// *|                See the project's LICENSE.md for details.                  --
// *|           Copyright Thomson Reuters 2017. All rights reserved.            --
// *|     It has not been tested for a usage in production environments.        --
///*|-----------------------------------------------------------------------------

#include "Consumer.h"
#include "fids.h"
#include <vector>
#include <algorithm>

using namespace thomsonreuters::ema::access;
using namespace thomsonreuters::ema::rdm;
using namespace std;

EmaString hostAndPort;
EmaString userName;
EmaString svcName;
EmaString itemName;

void AppClient::onRefreshMsg( const RefreshMsg& refreshMsg, const OmmConsumerEvent& ) 
{
	cout << "Refresh : " << (refreshMsg.hasName() ? refreshMsg.getName() : EmaString("<not set>"))
		<< "\tService: " << (refreshMsg.hasServiceName() ? refreshMsg.getServiceName() : EmaString("<not set>"))
		<< "\tDomain: " << ((refreshMsg.getDomainType() == MMT_MARKET_BY_ORDER) ? EmaString("MBO") : EmaString("MBP")) 
		<< ((refreshMsg.getComplete()) ? EmaString("\tOrderBook Complete - Final part received.") : EmaString("\tAdditional parts to come")) << endl;

	if ( DataType::MapEnum == refreshMsg.getPayload().getDataType() )
		decode( refreshMsg.getPayload().getMap() , refreshMsg.getDomainType());

	if (refreshMsg.getComplete())
	{
		if (refreshMsg.getDomainType() == MMT_MARKET_BY_ORDER)
		{
			std::vector<MBOEntry> mboVector;
			mboCache.asVector(mboVector);
			std::vector<MBOEntry>::iterator vit;
			cout << "DUMP MBO cache" << endl;
			for (vit = mboVector.begin(); vit < mboVector.end(); vit++)
				cout << vit->toString() << endl;
		}
		else if (refreshMsg.getDomainType() == MMT_MARKET_BY_PRICE)
		{
			std::vector<MBPEntry> mbpVector;
			mbpCache.asVector(mbpVector);
			std::vector<MBPEntry>::iterator vit;
			cout << "DUMP MBP cache" << endl;
			for (vit = mbpVector.begin(); vit < mbpVector.end(); vit++)
				cout << vit->toString() << endl;
		}
	}
}

void AppClient::onUpdateMsg( const UpdateMsg& updateMsg, const OmmConsumerEvent& ) 
{
	cout << "Update : " << ( updateMsg.hasName() ? updateMsg.getName() : EmaString( "<not set>" ) ) 
		<< "\tService: " << (updateMsg.hasServiceName() ? updateMsg.getServiceName() : EmaString( "<not set>" ) ) 
		<< "\tDomain: " << ((updateMsg.getDomainType() == MMT_MARKET_BY_ORDER) ? EmaString("MBO") : EmaString("MBP")) << endl;

	if ( DataType::MapEnum == updateMsg.getPayload().getDataType() )
		decode( updateMsg.getPayload().getMap() , updateMsg.getDomainType());
}

void AppClient::onStatusMsg( const StatusMsg& statusMsg, const OmmConsumerEvent& )
{
	cout << "Status for : " << ( statusMsg.hasName() ? statusMsg.getName() : EmaString( "<not set>" ) ) 
		<< "\tService Name: " << (statusMsg.hasServiceName() ? statusMsg.getServiceName() : EmaString( "<not set>" ) ) 
		<< "\tDomain: " << ((statusMsg.getDomainType() == MMT_MARKET_BY_ORDER) ? EmaString("MBO") : EmaString("MBP")) << endl;

	if ( statusMsg.hasState() )
		cout << endl << "Item State: " << statusMsg.getState().toString() << endl;
}


// Decode the SummaryData attached to the Map Payload
void AppClient::decodeSummary(const FieldList &fl, OBSummary &sd)
{
	unsigned int fieldFlags = 0;
	const unsigned int DsplyNameMask = 1;
	const unsigned int ExchIDMask = 2;
	const unsigned int PrRnkMask = 4;
	const unsigned int OrRnkMask = 8;
	const unsigned int TimActMsMask = 16;
	const unsigned int CurrencyMask = 32;
	const unsigned int ActivDateMask = 64;
	const unsigned int allSDFlagsSet = 127;

	try {
		while ( fl.forth() )
		{
			const FieldEntry& fe = fl.getEntry();

			switch (fe.getFieldId())
			{
			case fids::DSPLY_NAME:
				sd.displayName = fe.getRmtes().toString();
				fieldFlags |= DsplyNameMask;
				break;
			case fids::RDN_EXCHD2:
				sd.exchID = fe.getEnum();
				fieldFlags |= ExchIDMask;
				break;
			case fids::CURRENCY:
				sd.currency = fe.getEnum();
				fieldFlags |= CurrencyMask;
				break;
			case fids::ACTIV_DATE:
				sd.activDate = fe.getDate().toString();
				sd.AdYear = fe.getDate().getYear();
				sd.AdMonth = fe.getDate().getMonth();
				sd.AdDay = fe.getDate().getDay();
				fieldFlags |= ActivDateMask;
				break;
			case fids::TIMACT_MS:
				sd.timActMs = fe.getLoad().toString();
				fieldFlags |= TimActMsMask;
				break;
			case fids::PR_RNK_RUL:
				sd.prRnkRul = fe.getEnum();
				fieldFlags |= PrRnkMask;
				break;
			case fids::OR_RNK_RUL:
				sd.orRnkRul = fe.getEnum();
				fieldFlags |= OrRnkMask;
				break;
			}
		}
	}
	catch (const OmmException& excp) {
		cout << excp << endl;
	}

}

// Decode a MarketByOrder FieldList payload
bool AppClient::decodeMBOEntry(const FieldList &fl, MBOEntry &mboEntry)
{
	unsigned int fieldFlags = 0;			// Bitmask to track fields present in payload
	const unsigned int OrderPrcMask = 1;
	const unsigned int OrderSideMask = 2;
	const unsigned int OrderIDMask = 4;
	const unsigned int OrderSizeMask = 8;
	const unsigned int PrTimMsMask = 16;
	const unsigned int allMBOFlagsSet = 31;	// Bitmask for Mandatory fields

	MBOEntry mbo = mboEntry;	// Initialise temp object with existing cached field values (if any)

	try
	{
		while (fl.forth())
		{
			const FieldEntry& fe = fl.getEntry();

			switch (fe.getFieldId())
			{
			case fids::ORDER_PRC:
				mbo.OrderPrc = fe.getReal().getAsDouble();
				fieldFlags |= OrderPrcMask;
				break;
			case fids::ORDER_SIDE:
				mbo.OrderSide = fe.getEnum();
				mbo.OrderSideChar = (mbo.OrderSide == 1) ? 'B' : 'A';
				fieldFlags |= OrderSideMask;
				break;
			case fids::ORDER_ID:
				mbo.OrderID = fe.getRmtes().toString();
				fieldFlags |= OrderIDMask;
				break;
			case fids::ORDER_SIZE:
				mbo.OrderSize = fe.getReal().getAsDouble();
				fieldFlags |= OrderSizeMask;
				break;
			case fids::PR_TIM_MS:
				mbo.PrTimMs = fe.getUInt();
				fieldFlags |= PrTimMsMask;
				break;
			}

		}
	}
	catch (const OmmException& excp) {
		cout << excp << endl;
	}

	
	if (fieldFlags == allMBOFlagsSet)	// If all mandatory fields present in FieldList 
		mboEntry = mbo;					// copy fields from temp object into cache entry

	return (fieldFlags == allMBOFlagsSet);

}

// Decode a MarketByPrice FieldList payload
bool AppClient::decodeMBPEntry(const FieldList &fl, MBPEntry &mbpEntry, MapEntry::MapAction action)
{
	unsigned int fieldFlags = 0;					// Bitmask to track fields present in payload
	const unsigned int OrderPrcMask = 1;
	const unsigned int OrderSideMask = 2;
	const unsigned int NoOrdMask = 4;
	const unsigned int AccSizeMask = 8;
	const unsigned int LvTimMsMask = 16;
	const unsigned int LvDateMask = 32;
	const unsigned int allMBPAddFlagsSet = 63;		// Mandatory fields for Add action
	const unsigned int allMBPUpdateFlagsSet = 17;	// Mandatory fields for Update action

	MBPEntry mbp = mbpEntry;	// Initialise temp object with existing cached field values (if any)

	try
	{
		while (fl.forth())
		{
			const FieldEntry& fe = fl.getEntry();

			switch (fe.getFieldId())
			{
			case fids::ORDER_PRC:
				mbp.OrderPrc = fe.getReal().getAsDouble();
				fieldFlags |= OrderPrcMask;
				break;
			case fids::ORDER_SIDE:
				mbp.OrderSide = fe.getEnum();
				mbp.OrderSideChar = (mbp.OrderSide == 1) ? 'B' : 'A';
				fieldFlags |= OrderSideMask;
				break;
			case fids::NO_ORD:
				mbp.NoOrd = fe.getUInt();
				fieldFlags |= NoOrdMask;
				break;
			case fids::ACC_SIZE:
				mbp.AccSize = fe.getReal().getAsDouble();
				fieldFlags |= AccSizeMask;
				break;
			case fids::LV_TIM_MS:
				mbp.LvTimMs = fe.getUInt();
				fieldFlags |= LvTimMsMask;
				break;
			case fids::LV_DATE:
				mbp.LvYear = fe.getDate().getYear();
				mbp.LvMonth = fe.getDate().getMonth();
				mbp.LvDay = fe.getDate().getDay();
				fieldFlags |= LvDateMask;
				break;
			case fids::PR_TIM_MS:
				mbp.PrTimMs = fe.getUInt();
				break;
			}

		}
	}
	catch (const OmmException& excp) {
		cout << excp << endl;
	}

	// For Row Entry Add - all mandatory fields must be present
	if (action == MapEntry::AddEnum)
	{
		if (fieldFlags == allMBPAddFlagsSet)	// All mandatory fields present in FieldList 
			mbpEntry = mbp;						// so just copy all fields to cached entry
		return (fieldFlags == allMBPAddFlagsSet);
	}
	else
	{	// Row Update - fewer Mandatory fields required to accept update
		if ((fieldFlags & allMBPUpdateFlagsSet) == allMBPUpdateFlagsSet)	// If Mandatory fields are present
			mbpEntry = mbp;													// Copy all fields to cached entry
		return ((fieldFlags & allMBPUpdateFlagsSet) == allMBPUpdateFlagsSet);
	}

}

// Decode the Map payload
void AppClient::decode( const Map& map , UInt16 domainType)
{
	// Firstly let us extract any Summary Data present in the Map
	// A Map delivered in a RefreshMsg should always contains a SummaryData section. 
	// An UpdateMsg can also contain a Summary Data section
	if ( map.getSummaryData().getDataType() == DataType::FieldListEnum )
	{
		if (domainType == MMT_MARKET_BY_ORDER)
			decodeSummary(map.getSummaryData().getFieldList(), mboCache.summaryData);
		else if (domainType == MMT_MARKET_BY_PRICE)
			decodeSummary(map.getSummaryData().getFieldList(), mbpCache.summaryData);
	}
		
	while ( map.forth() )
	{
		const MapEntry& me = map.getEntry();

		DataType::DataTypeEnum dataType = map.getEntry().getLoadType();
		const EmaBuffer& keyBuff = map.getEntry().getKey().getBuffer();
		std::string key(keyBuff.c_buf(), keyBuff.length());
		MapEntry::MapAction entryAction = me.getAction();
		// Map entries with Add Action or Update action should have FieldList payload
		if ( (dataType == DataType::FieldListEnum) && (entryAction == MapEntry::AddEnum || entryAction == MapEntry::UpdateEnum))
		{
			if (domainType == MMT_MARKET_BY_ORDER)
			{
				MBOEntry & mbo = mboCache.entries[key];		// If Key exists get entry - otherwise Add new entry
				if (!decodeMBOEntry(me.getFieldList(), mbo) && entryAction == MapEntry::AddEnum)
					mboCache.entries.erase(key);	// All mandatory fields must exist for Add - otherwise delete newly Added entry
			}
			else if (domainType == MMT_MARKET_BY_PRICE)
			{
				MBPEntry & mbp = mbpCache.entries[key];		// If Key exists get entry - otherwise Add new entry
				if (!decodeMBPEntry(me.getFieldList(), mbp, me.getAction()) && entryAction == MapEntry::AddEnum)	// Mandatory fields differ for Add / Update type Action
					mbpCache.entries.erase(key);	// Mandatory field(s) missing for Add, so delete entry
			}
		}
		else
		{	// Map entries with Delete Action should have no data payload
			if ((dataType == DataType::NoDataEnum) &&  (me.getAction() == MapEntry::DeleteEnum))
			{	
				if (domainType == MMT_MARKET_BY_ORDER)
					mboCache.entries.erase(key);
				else if (domainType == MMT_MARKET_BY_PRICE)
					mbpCache.entries.erase(key);
			}
		}
	}
}

// Output MBO and MBP items in sorted / ranked order
void AppClient::dumpSorted()
{
	// Sort MarketByOrder
	// Get copy of cached MBO in Vector so we can sort it
	std::vector<MBOEntry>::iterator vitMBO;
	std::vector<MBOEntry> bidsMBO, asksMBO;
	mboCache.asBidAskVectors(bidsMBO, asksMBO);

	// Ouput unsorted for comparison
	cout << "Dump MBO BIDs" << endl;
	for (vitMBO = bidsMBO.begin(); vitMBO < bidsMBO.end(); vitMBO++)
		cout << vitMBO->toString() << endl;
	cout << "Dump MBO ASKs" << endl;
	for (vitMBO = asksMBO.begin(); vitMBO < asksMBO.end(); vitMBO++)
		cout << vitMBO->toString() << endl;

	OBSummary mbosd = mboCache.summaryData;

	// Sort the MBO vectors
	switch (mbosd.prRnkRul)
	{
	case fids::PrRnkRul_NOR:
		switch (mbosd.orRnkRul)
		{
		case fids::OrRnkRul_PTS:
			std::sort(bidsMBO.begin(), bidsMBO.end(), MBOEntry::PTS_descendingPrice());
			std::sort(asksMBO.begin(), asksMBO.end(), MBOEntry::PTS_ascendingPrice());
			break;
		case fids::OrRnkRul_PST:
			std::sort(bidsMBO.begin(), bidsMBO.end(), MBOEntry::PST_descendingPrice());
			std::sort(asksMBO.begin(), asksMBO.end(), MBOEntry::PST_ascendingPrice());
			break;
		}
		break;
	case fids::PrRnkRul_INV:
		switch (mbosd.orRnkRul)
		{
		case fids::OrRnkRul_PTS:
			std::sort(bidsMBO.begin(), bidsMBO.end(), MBOEntry::PTS_ascendingPrice());
			std::sort(asksMBO.begin(), asksMBO.end(), MBOEntry::PTS_descendingPrice());
			break;
		case fids::OrRnkRul_PST:
			std::sort(bidsMBO.begin(), bidsMBO.end(), MBOEntry::PST_ascendingPrice());
			std::sort(asksMBO.begin(), asksMBO.end(), MBOEntry::PST_descendingPrice());
			break;
		}
		break;
	}

	// Ouput sorted MBO
	cout << "Dump MBO SORTED BIDs" << endl;
	for (vitMBO = bidsMBO.begin(); vitMBO < bidsMBO.end(); vitMBO++)
		cout << vitMBO->toString() << endl;
	cout << "Dump MBO SORTED ASKs" << endl;
	for (vitMBO = asksMBO.begin(); vitMBO < asksMBO.end(); vitMBO++)
		cout << vitMBO->toString() << endl;

	// Sort MarketByPrice
	// Get copy of cached MBP in Vector so we can sort it
	std::vector<MBPEntry>::iterator vitMBP;
	std::vector<MBPEntry> bidsMBP, asksMBP;
	mbpCache.asBidAskVectors(bidsMBP, asksMBP);

	// Ouput unsorted for comparison
	cout << "Dump MBP BID rows/entrys" << endl;
	for (vitMBP = bidsMBP.begin(); vitMBP < bidsMBP.end(); vitMBP++)
		cout << vitMBP->toString() << endl;

	cout << "Dump MBP ASK rows/entrys" << endl;
	for (vitMBP = asksMBP.begin(); vitMBP < asksMBP.end(); vitMBP++)
		cout << vitMBP->toString() << endl;


	OBSummary mbpsd = mbpCache.summaryData;

	// Sort the MPB vectors
	switch (mbpsd.prRnkRul)
	{
	case fids::PrRnkRul_NOR:
		switch (mbpsd.orRnkRul)
		{
		case fids::OrRnkRul_PTS:
			std::sort(bidsMBP.begin(), bidsMBP.end(), MBPEntry::PTS_descendingPrice());
			std::sort(asksMBP.begin(), asksMBP.end(), MBPEntry::PTS_ascendingPrice());
			break;
		case fids::OrRnkRul_PST:
			std::sort(bidsMBP.begin(), bidsMBP.end(), MBPEntry::PST_descendingPrice());
			std::sort(asksMBP.begin(), asksMBP.end(), MBPEntry::PST_ascendingPrice());
			break;
		}
		break;
	case fids::PrRnkRul_INV:
		switch (mbpsd.orRnkRul)
		{
		case fids::OrRnkRul_PTS:
			std::sort(bidsMBP.begin(), bidsMBP.end(), MBPEntry::PTS_ascendingPrice());
			std::sort(asksMBP.begin(), asksMBP.end(), MBPEntry::PTS_descendingPrice());
			break;
		case fids::OrRnkRul_PST:
			std::sort(bidsMBP.begin(), bidsMBP.end(), MBPEntry::PST_ascendingPrice());
			std::sort(asksMBP.begin(), asksMBP.end(), MBPEntry::PST_descendingPrice());
			break;
		}
		break;
	}

	// Ouput sorted MBP
	cout << "Dump MBP SORTED BID rows/entrys" << endl;
	for (vitMBP = bidsMBP.begin(); vitMBP < bidsMBP.end(); vitMBP++)
		cout << vitMBP->toString() << endl;
	cout << "Dump MBP SORTED ASK rows/entrys" << endl;
	for (vitMBP = asksMBP.begin(); vitMBP < asksMBP.end(); vitMBP++)
		cout << vitMBP->toString() << endl;

}


void printUsage()
{
	cout << "\ncommand line arguments:\n"
		<< "  -?				Shows this usage\n\n"
		<< "  -h <host IP/name:port>	Host IP (or name) and port e.g. 10.13.123.55:14002 or ads1:14002\n"
		<< "  -u <username>			DACS ID to use for Login to server\n"
		<< "  -s <service name>		Service to request data name from e.g. ELEKTRON_DD\n"
		<< "  -i <item RIC code>		RIC code for item to request MBP and MBO data for e.g. VOD.L\n"
		<< "\n";
	exit(-1);
}

void processCommandLineOptions(int argc, char* argv[])
{

	if (argc < 9)
		printUsage();

	int iargs = 1;

	while (iargs < argc)
	{
		if (0 == strcmp("-?", argv[iargs]))
		{
			printUsage();
		}
		else if (strcmp("-h", argv[iargs]) == 0)
		{
			++iargs; if (iargs == argc) printUsage();

			hostAndPort.set(argv[iargs++]);
		}
		else if (strcmp("-u", argv[iargs]) == 0)
		{
			++iargs; if (iargs == argc) printUsage();

			userName.set(argv[iargs++]);
		}
		else if (strcmp("-s", argv[iargs]) == 0)
		{
			++iargs; if (iargs == argc) printUsage();

			svcName.set(argv[iargs++]);
		}
		else if (strcmp("-i", argv[iargs]) == 0)
		{
			++iargs; if (iargs == argc) printUsage();

			itemName.set(argv[iargs++]);
		}
		else
		{
			cout << "Invalid argument: " << argv[iargs] << endl;

			printUsage();
		}

	}

	if (userName.empty() || svcName.empty() || hostAndPort.empty() || itemName.empty())
		printUsage();

}


int main( int argc, char* argv[] )
{ 
	try { 

		processCommandLineOptions(argc, argv);

		AppClient client;
		OmmConsumer consumer( OmmConsumerConfig().host( hostAndPort ).username( userName) );
		consumer.registerClient( ReqMsg().domainType( MMT_MARKET_BY_ORDER ).serviceName( svcName ).name( itemName ), client );
		consumer.registerClient( ReqMsg().domainType( MMT_MARKET_BY_PRICE ).serviceName( svcName ).name( itemName ), client);
		// Loop for a while
		for (UInt32 idx = 0; idx < 300; ++idx)
		{
			sleep(10000);			// Sleep 10 seconds for updates
			client.dumpSorted();	// Sort and output orderbook	
		}

	} catch ( const OmmException& excp ) {
		cout << excp << endl;
	}
	return 0;
}
