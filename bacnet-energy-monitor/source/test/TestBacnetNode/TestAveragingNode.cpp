/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestAveragingNode.h"

TestAveragingNode::TestAveragingNode(){
	bacnetNode = new AveragingNode();
	testBacnetNodeDriver = new TestBacnetNodeDriver();
	testBacnetNodeStub = new TestBacnetNodeStub();
}

TestAveragingNode::~TestAveragingNode(){
	delete(bacnetNode);
	delete(testBacnetNodeDriver);
	delete(testBacnetNodeStub);
}


void TestAveragingNode::nothing(){
	ASSERT(true);
}

void TestAveragingNode::testAveragingNodeHandlerReadObjectName(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_DEVICE;
	data.object_instance = 200;
	data.object_property = PROP_OBJECT_NAME;
	data.array_index = 0;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	BACNET_CHARACTER_STRING name;
	characterstring_init_ansi(&name, "MiguelDevice");
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, &name));
}

void TestAveragingNode::testAveragingNodeHandlerReadAverageValue(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_AVERAGING;
	data.object_instance = 56;
	data.object_property = PROP_AVERAGE_VALUE;
	data.array_index = BACNET_ARRAY_ALL;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, isnanf(appDataValueOUT.type.Real) != 0);

}

void TestAveragingNode::testAveragingNodeHandlerReadAccessNonExistingObject(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_ACCUMULATOR;
	data.object_instance = 32;
	data.object_property = PROP_AVERAGE_VALUE;
	data.array_index = BACNET_ARRAY_ALL;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);
	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);

	ASSERT_EQUAL(ERROR_CLASS_OBJECT, (BACNET_ERROR_CLASS)data.error_class);
	ASSERT_EQUAL(ERROR_CODE_UNKNOWN_OBJECT, (BACNET_ERROR_CODE)data.error_code);
}

void TestAveragingNode::testAveragingNodeHandlerReadNonExistingProperty(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_AVERAGING;
	data.object_instance = 32;
	data.object_property = PROP_ACCESS_DOORS;
	data.array_index = BACNET_ARRAY_ALL;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);
	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);

	ASSERT_EQUAL(ERROR_CLASS_PROPERTY, (BACNET_ERROR_CLASS)data.error_class);
	ASSERT_EQUAL(ERROR_CODE_UNKNOWN_PROPERTY, (BACNET_ERROR_CODE)data.error_code);
}

void TestAveragingNode::testAveragingNodeHandlerSendingSegmentedMessage(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;
	uint8_t apdu[MAX_APDU + 10] = { 1 };

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_AVERAGING;
	data.object_instance = 32;
	data.object_property = PROP_ACCESS_DOORS;
	data.array_index = BACNET_ARRAY_ALL;
	data.application_data = &apdu[0];
	data.application_data_len = MAX_APDU + 10;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);
	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);

	//ASSERT_EQUAL(ERROR_CLASS_, (BACNET_ERROR_CLASS)data.error_class);
	ASSERT_EQUAL(ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED, (BACNET_ERROR_CODE)data.error_code);
}

void TestAveragingNode::testAveragingReadAverageAfterInserting1Value(){
	// Add Value
	float value = 5.0;
	bacnetNode->makeNewMeasurement(value);

	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_AVERAGING;
	data.object_instance = 56;
	data.object_property = PROP_AVERAGE_VALUE;
	data.array_index = BACNET_ARRAY_ALL;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(value, appDataValueOUT.type.Real);
}

void TestAveragingNode::testAveragingReadAverageMaximumMinimumAfterInserting50Values(){
	//Add Values
	float value = -6;
	for (int i = 0; i < 50; i++){
		bacnetNode->makeNewMeasurement(value);
		value += (float)1.0;
	}

	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - AVERAGE VALUE */
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_AVERAGING;
	data.object_instance = 56;
	data.object_property = PROP_AVERAGE_VALUE;
	data.array_index = BACNET_ARRAY_ALL;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, abs(appDataValueOUT.type.Real - (float)21.0) < 1E-10);

	/* Configure Request - MAXIMUM VALUE */
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_AVERAGING;
	data.object_instance = 56;
	data.object_property = PROP_MAXIMUM_VALUE;
	data.array_index = BACNET_ARRAY_ALL;

	pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, abs(appDataValueOUT.type.Real - (float)43.0) < 1E-10);

	/* Configure Request - MINIMUM VALUE */
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_AVERAGING;
	data.object_instance = 56;
	data.object_property = PROP_MINIMUM_VALUE;
	data.array_index = BACNET_ARRAY_ALL;

	pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
			data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, abs(appDataValueOUT.type.Real - (float)-1.0) < 1E-10);

}
