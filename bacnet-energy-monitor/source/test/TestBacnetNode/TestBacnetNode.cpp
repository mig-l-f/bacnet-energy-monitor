/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestBacnetNode.h"

TestBacnetNode::TestBacnetNode(){
	bacnetNode = new BacnetNode();
	testBacnetNodeDriver = new TestBacnetNodeDriver();
	testBacnetNodeStub = new TestBacnetNodeStub();
}

TestBacnetNode::~TestBacnetNode(){
	delete(testBacnetNodeDriver);
	delete(testBacnetNodeStub);
	delete(bacnetNode);

}

void TestBacnetNode::nothing(){
	ASSERT_EQUAL(true, true);
}
void TestBacnetNode::testBacnetNodeHasDeviceObject(){
	if(bacnetNode->getDeviceObject() != NULL)
		ASSERT_EQUAL(true,true);
	else
		ASSERT_EQUAL(true, false);
}
void TestBacnetNode::testBacnetNodeHandlerWhoIsWithLimits(){
	int32_t low_limit = 50;
	int32_t high_limit = 60;
	int len = 0;
	BACNET_NPDU_DATA npdu_data;
	BACNET_NPDU_DATA npdu_dataAtServer;
	BACNET_NPDU_DATA npdu_dataReceivedAtClient;
	BACNET_ADDRESS dest = { 0 }; // Broadcast address
	dest.mac_len = 0;
	dest.len = 0;

	// Client Sends Npdu
	len = testBacnetNodeDriver->sendWhoIsNpdu(npdu_data, dest, low_limit, high_limit);

	//Server reception
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_dataAtServer, len);

	// Client Side
	len = 26;
	int apdu_offset = testBacnetNodeStub->handlerNpduIamAtClient(&Handler_Transmit_Buffer[0], npdu_dataReceivedAtClient, len);


	if (apdu_offset != 1){
		// Output
		uint32_t device_id = 0;
		unsigned max_apdu = 0;
		int segmentation = 0;
		uint16_t vendor_id = 0;
		if (Handler_Transmit_Buffer[apdu_offset] != PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST){
			ASSERT_EQUALM("APDU Nao foi bem descodificada", true, false);
		}
		if (Handler_Transmit_Buffer[apdu_offset+1] != SERVICE_UNCONFIRMED_I_AM){
			ASSERT_EQUALM("APDU Nao foi bem descodificada", true, false);
		}
		len = iam_decode_service_request(&Handler_Transmit_Buffer[apdu_offset+2], &device_id, &max_apdu,
				&segmentation, &vendor_id);
		ASSERT_EQUAL(bacnetNode->getDeviceObject()->getObjectIdentifier()->instance, device_id);
		ASSERT_EQUAL(MAX_APDU, max_apdu);
		ASSERT_EQUAL(SEGMENTATION_NONE, segmentation);
		ASSERT_EQUAL(VENDOR_IDENTIFIER, vendor_id);
	}else{
		ASSERT_EQUALM("APDU Nao foi descodificada", true, false);
	}
}
void TestBacnetNode::testBacnetNodeHandlerWhoIsWithWildcard(){
	int32_t low_limit = BACNET_MAX_INSTANCE;
	int32_t high_limit = BACNET_MAX_INSTANCE;
	int len = 0;
	BACNET_NPDU_DATA npdu_data;
	BACNET_NPDU_DATA npdu_dataAtServer;
	BACNET_NPDU_DATA npdu_dataReceivedAtClient;
	BACNET_ADDRESS dest = { 0 }; // Broadcast address
	dest.mac_len = 0;
	dest.len = 0;

	// Client Sends Npdu
	len = testBacnetNodeDriver->sendWhoIsNpdu(npdu_data, dest, low_limit, high_limit);

	//Server reception
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_dataAtServer, len);

	// Client Side
	len = 26;
	int apdu_offset = testBacnetNodeStub->handlerNpduIamAtClient(&Handler_Transmit_Buffer[0], npdu_dataReceivedAtClient, len);


	if (apdu_offset != 1){
		// Output
		uint32_t device_id = 0;
		unsigned max_apdu = 0;
		int segmentation = 0;
		uint16_t vendor_id = 0;
		if (Handler_Transmit_Buffer[apdu_offset] != PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST){
			ASSERT_EQUALM("APDU Nao foi bem descodificada", true, false);
		}
		if (Handler_Transmit_Buffer[apdu_offset+1] != SERVICE_UNCONFIRMED_I_AM){
			ASSERT_EQUALM("APDU Nao foi bem descodificada", true, false);
		}
		len = iam_decode_service_request(&Handler_Transmit_Buffer[apdu_offset+2], &device_id, &max_apdu,
				&segmentation, &vendor_id);
		ASSERT_EQUAL(bacnetNode->getDeviceObject()->getObjectIdentifier()->instance, device_id);
		ASSERT_EQUAL(MAX_APDU, max_apdu);
		ASSERT_EQUAL(SEGMENTATION_NONE, segmentation);
		ASSERT_EQUAL(VENDOR_IDENTIFIER, vendor_id);
	}else{
		ASSERT_EQUALM("APDU Nao foi descodificada", true, false);
	}
}
void TestBacnetNode::testBacnetNodeHandlerWhoIsWithNoLimits(){
	int32_t low_limit = -1;
	int32_t high_limit = -1;
	int len = 0;
	BACNET_NPDU_DATA npdu_data;
	BACNET_NPDU_DATA npdu_dataAtServer;
	BACNET_NPDU_DATA npdu_dataReceivedAtClient;
	BACNET_ADDRESS dest = { 0 }; // Broadcast address
	dest.mac_len = 0;
	dest.len = 0;

	// Client Sends Npdu
	len = testBacnetNodeDriver->sendWhoIsNpdu(npdu_data, dest, low_limit, high_limit);

	//Server reception
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_dataAtServer, len);

	// Client Side
	len = 26;
	int apdu_offset = testBacnetNodeStub->handlerNpduIamAtClient(&Handler_Transmit_Buffer[0], npdu_dataReceivedAtClient, len);


	if (apdu_offset != 1){
		// Output
		uint32_t device_id = 0;
		unsigned max_apdu = 0;
		int segmentation = 0;
		uint16_t vendor_id = 0;
		if (Handler_Transmit_Buffer[apdu_offset] != PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST){
			ASSERT_EQUALM("APDU Nao foi bem descodificada", true, false);
		}
		if (Handler_Transmit_Buffer[apdu_offset+1] != SERVICE_UNCONFIRMED_I_AM){
			ASSERT_EQUALM("APDU Nao foi bem descodificada", true, false);
		}
		len = iam_decode_service_request(&Handler_Transmit_Buffer[apdu_offset+2], &device_id, &max_apdu,
				&segmentation, &vendor_id);
		ASSERT_EQUAL(bacnetNode->getDeviceObject()->getObjectIdentifier()->instance, device_id);
		ASSERT_EQUAL(MAX_APDU, max_apdu);
		ASSERT_EQUAL(SEGMENTATION_NONE, segmentation);
		ASSERT_EQUAL(VENDOR_IDENTIFIER, vendor_id);
	}else{
		ASSERT_EQUALM("APDU Nao foi descodificada", true, false);
	}
}
