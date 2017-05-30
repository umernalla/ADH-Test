Summary
=======

OrderBookSort is an example of an OMM Consumer application
that demonstrates basic usage of the EMA library to access, parse and sort OMM 
MarketByOrder / MarketByPrice data from the Reuters Data Feed Direct (RDF-D), directly from 
an OMM Provider application, or from an Advanced Distribution Server.

OrderBookSort illustrates how to open and process a MarketByOrder / MarketByPrice item. 
While processing received messages, the application iterates through the received Map and FieldList, 
caches the content whilst sorting and maintaining the OrderBooks.
Every 10 seconds it prints the sorted Orderbooks out to the console.


Detailed Description
====================

OrderBookSort implements the following high-level steps:

In the main() method:

    1 Login to the server
    2 Request a MarketByPrice and MarketByOrder instrument and register the same AppClient instance to process the response messages / data for both
    3 Loop and dump both Orderbooks in ranked order every 10 seconds

In the RefreshMsg handler:

   1 Perform basic validation, extract the Map and pass it to the decode method to build the local cache of each Orderbook
   2 When the complete Orderbook has been received, dump the cached to the console in the received (unsorted) order

In the UpdateMsg handler, perform some basic validation, extract the Map and pass to the decode method to update the local cache of the relevant Orderbook (MBP or MBO)

In the decode() method:

   1 If the Map has any SummaryData, extract it and update the SummaryData in the local cache for the corresponding Orderbook
   2 Iterate through the Map entries and extract each entry’s Key
   3 If the Map entry’s Action type is Add or Update extract the FieldList payload
	a)  Decode the FieldList payload and ensure all the mandatory fields are present
	b)  *Add* or *Update* the entry to the local cache using the extracted Key
   4 If the Map entry’s Action type is Delete then erase the corresponding entry from the local cache using the entry’s Key

In the decodeMBPEntry() and decodeMBOEntry() methods:
   1 Iterate through the FieldList, extract the mandatory (minimum) fields
   2 If all mandatory fields exist in the FieldList then apply to the cache entry
   3 Return true if all mandatory fields exist in the FieldList or false if any field is missing

One thing to bear in mind with the example code is that it is purely for illustration purposes and should not be used for production purposes.