/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestAnalogValueNode.h"

TestAnalogValueNode::TestAnalogValueNode(){
	bacnetNode = new AnalogValueNode();
	testBacnetNodeDriver = new TestBacnetNodeDriver();
	testBacnetNodeStub = new TestBacnetNodeStub();
}

TestAnalogValueNode::~TestAnalogValueNode(){
	delete(bacnetNode);
	delete(testBacnetNodeDriver);
	delete(testBacnetNodeStub);
}

void TestAnalogValueNode::nothing(){
	ASSERT(true);
}

void TestAnalogValueNode::testNodeHandlerReadPropertyObjectName(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_DEVICE;
	data.object_instance = 22;
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

void TestAnalogValueNode::testNodeHandlerReadProperyPresentValue(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_ANALOG_VALUE;
	data.object_instance = 57;
	data.object_property = PROP_PRESENT_VALUE;
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
	ASSERT_EQUAL(-1, appDataValueOUT.type.Real);
}

void TestAnalogValueNode::testNodeHandlerReadAccessNonExistingObject(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_ACCESS_POINT;
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

void TestAnalogValueNode::testNodeHandlerReadNonExistingProperty(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_ANALOG_VALUE;
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

void TestAnalogValueNode::testNodeReadPresentValueAfterInserting1Value(){
	// Add Value
	float value = 5.0;
	bacnetNode->setPresentValue(value);

	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request - Sender?*/
	data.object_type = (BACNET_OBJECT_TYPE)OBJECT_ANALOG_VALUE;
	data.object_instance = 56;
	data.object_property = PROP_PRESENT_VALUE;
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

