// Field IDs for all fields of interest to the LL2 Adaptor.

#ifndef FIDS_H
#define FIDS_H

#include "Ema.h"


namespace fids {

	// FIDs that we want to copy from MBP/MBO response message summary data
	const thomsonreuters::ema::access::Int16 DSPLY_NAME = 3;
	const thomsonreuters::ema::access::Int16 CURRENCY = 15;
	const thomsonreuters::ema::access::Int16 ACTIV_DATE = 17;
	const thomsonreuters::ema::access::Int16 RECORDTYPE = 259;
	const thomsonreuters::ema::access::Int16 RDN_EXCHD2 = 1709;
	const thomsonreuters::ema::access::Int16 TIMACT_MS = 4148;  // Special treatment. Converted to TIMACT

	// FIDs in the MBP summary data used in the conversion process
	const thomsonreuters::ema::access::Int16 PR_RNK_RUL = 3423;  // Price rank rule
	enum PriceRankRule { PrRnkRul_UNDEF, PrRnkRul_NOR, PrRnkRul_INV };
	const thomsonreuters::ema::access::Int16 OR_RNK_RUL = 3425;  // Order rank rule
	enum OrderRankRule { OrRnkRul_UNDEF, OrRnkRul_PST, OrRnkRul_PTS, OrRnkRul_PRS };

	// FIDs from the map entries of the MBP response message payload
	const thomsonreuters::ema::access::Int16 ORDER_PRC = 3427;
	const thomsonreuters::ema::access::Int16 ORDER_SIDE = 3428;
	enum OrderSide { OrderSide_UNDEF, OrderSide_BID, OrderSide_ASK };
	const thomsonreuters::ema::access::Int16 NO_ORD = 3430;
	const thomsonreuters::ema::access::Int16 ACC_SIZE = 4356;
	const thomsonreuters::ema::access::Int16 LV_TIM_MS = 6527;
	const thomsonreuters::ema::access::Int16 LV_DATE = 6529;

	// Additional FIDs from the map entries of the MBO response message payload
	const thomsonreuters::ema::access::Int16 ORDER_ID = 3426;
	const thomsonreuters::ema::access::Int16 ORDER_SIZE = 3429;
	const thomsonreuters::ema::access::Int16 PR_TIM_MS = 6520;

	// Legacy Level 2 FIDs in the Market Price message


}

#endif  // FIDS_H
