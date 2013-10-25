/*
 * TestBacnetNode.cpp
 *
 *  Created on: 22 de Set de 2013
 *      Author: mgf
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
void TestBacnetNode::testBacnetNodeHandlerReadObjectName(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = (BACNET_OBJECT_TYPE)bacnetNode->getDeviceObject()->getObjectIdentifier()->type;
	data.object_instance = bacnetNode->getDeviceObject()->getObjectIdentifier()->instance;
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
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, bacnetNode->getDeviceObject()->getObjectName()));
}
void TestBacnetNode::testBacnetNodeHandlerReadSystemStatus(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = (BACNET_OBJECT_TYPE)bacnetNode->getDeviceObject()->getObjectIdentifier()->type;
	data.object_instance = bacnetNode->getDeviceObject()->getObjectIdentifier()->instance;
	data.object_property = PROP_SYSTEM_STATUS;
	data.array_index = 0;
	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address, data,
			bacnetNode, invoke_id, false);

	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE

	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data); // Descodificar no cliente de volta?

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(bacnetNode->getDeviceObject()->getDeviceStatus(),appDataValueOUT.type.Enumerated);
}
void TestBacnetNode::testBacnetNodeHandlerReadServicesSupported(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = (BACNET_OBJECT_TYPE)bacnetNode->getDeviceObject()->getObjectIdentifier()->type;
	data.object_instance = bacnetNode->getDeviceObject()->getObjectIdentifier()->instance;
	data.object_property = PROP_PROTOCOL_SERVICES_SUPPORTED;
	data.array_index = 0;
	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address, data,
			bacnetNode, invoke_id, false);

	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE

	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data); // Descodificar no cliente de volta?

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);

	// Write expected - Services Supported
	BACNET_BIT_STRING bitstring1;
	bitstring_init(&bitstring1);
	for(int i = 0; i < MAX_BACNET_SERVICES_SUPPORTED; i++){
		if ((i == SERVICE_SUPPORTED_READ_PROPERTY) || (i == SERVICE_SUPPORTED_WHO_IS)){
			bitstring_set_bit(&bitstring1, (uint8_t)i, true);
		}else{
			bitstring_set_bit(&bitstring1, (uint8_t)i, false);
		}
	}
	ASSERT_EQUAL(true, bitstring_same(&bitstring1,&appDataValueOUT.type.Bit_String)); //FIXME: No Bit_String
}
void TestBacnetNode::testBacnetNodeHandlerReadIndefiniteDeviceObjectInstance(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = (BACNET_OBJECT_TYPE)bacnetNode->getDeviceObject()->getObjectIdentifier()->type;
	data.object_instance = BACNET_MAX_INSTANCE;
	data.object_property = PROP_OBJECT_NAME;
	data.array_index = 0;
	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address, data,
			bacnetNode, invoke_id, false);
	// SENDING CLIENTE->SERVIDOR

	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE

	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data); // Descodificar no cliente de volta?

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(data.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, bacnetNode->getDeviceObject()->getObjectName()));
	ASSERT_EQUAL(bacnetNode->getDeviceObject()->getObjectIdentifier()->instance, data.object_instance);
}
void TestBacnetNode::testBacnetNodeHandlerReadReject(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = (BACNET_OBJECT_TYPE)bacnetNode->getDeviceObject()->getObjectIdentifier()->type;
	data.object_instance = bacnetNode->getDeviceObject()->getObjectIdentifier()->instance;
	data.object_property = (BACNET_PROPERTY_ID)(4194303+1);
	data.array_index = BACNET_ARRAY_ALL;
	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address, data,
				bacnetNode, invoke_id, true);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	BACNET_ADDRESS src = { 0 };
	int apdu_offset = npdu_decode(&Handler_Transmit_Buffer[0], &dest, &src, &npdu_data);
	uint8_t* apdu = &Handler_Transmit_Buffer[apdu_offset];
	if (apdu[2] == REJECT_REASON_MISSING_REQUIRED_PARAMETER){
		ASSERT(true);
	}else{
		ASSERT(false);
	}

	if (apdu[0] == PDU_TYPE_REJECT){
		ASSERT(true);
	}else{
		ASSERT(false);
	}
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
		ASSERT_EQUAL(bacnetNode->getDeviceObject()->getVendorIdentifier(), vendor_id);
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
		ASSERT_EQUAL(bacnetNode->getDeviceObject()->getVendorIdentifier(), vendor_id);
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
		ASSERT_EQUAL(bacnetNode->getDeviceObject()->getVendorIdentifier(), vendor_id);
	}else{
		ASSERT_EQUALM("APDU Nao foi descodificada", true, false);
	}
}
void TestBacnetNode::testBacnetNodeHandlerAnalogValueObjectName(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = OBJECT_ANALOG_VALUE;
	data.object_instance = (uint32_t)100;
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
	characterstring_init_ansi(&name, "TestAnalogValue");
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, &name));
}
void TestBacnetNode::testBacnetNodeHandlerAnalogValueReadPresentValue(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = OBJECT_ANALOG_VALUE;
	data.object_instance = (uint32_t)100;
	data.object_property = PROP_PRESENT_VALUE;
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
	ASSERT_EQUAL(-1.0, appDataValueOUT.type.Real);
}
void TestBacnetNode::testBacnetNodeHandlerDeviceObjectListOfObjects(){
	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	/* Configure Request */
	data.object_type = OBJECT_DEVICE;
	data.object_instance = (uint32_t)55;
	data.object_property = PROP_OBJECT_LIST;
	data.array_index = BACNET_ARRAY_ALL;

	int pdu_len = testBacnetNodeDriver->makeReadPropertyRequestStub(npdu_data, dest, my_address,
				data, bacnetNode, invoke_id, false);

	// SENDING CLIENTE->SERVIDOR
	testBacnetNodeDriver->handlerNpduStubAtServer(&Handler_Transmit_Buffer[0], npdu_data, pdu_len);

	// SENDING SERVIDOR->CLIENTE
	int pdu_len2 = 40; // Valor obtido atraves de debug na funcao myhandler_read (BacnetNode.cpp)
	testBacnetNodeStub->handlerNpduStubAtClient(&Handler_Transmit_Buffer[0], npdu_data, pdu_len2, &data);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	uint8_t* application_data = data.application_data;
	int application_data_len = data.application_data_len;
	int len = 0;
	len = bacapp_decode_application_data(application_data, (uint8_t)application_data_len, &appDataValueOUT);

	BACNET_OBJECT_ID obj1 = appDataValueOUT.type.Object_Id;
	ASSERT_EQUAL(55, obj1.instance);
	ASSERT_EQUAL(OBJECT_DEVICE, obj1.type);
	if (len > 0){
		if (len < application_data_len){
			application_data += len;
			application_data_len -= len;
			len = bacapp_decode_application_data(application_data, (uint8_t)application_data_len, &appDataValueOUT);
			obj1 = appDataValueOUT.type.Object_Id;
			ASSERT_EQUAL(100, obj1.instance);
			ASSERT_EQUAL(OBJECT_ANALOG_VALUE, obj1.type);
		}
	}
}

void TestBacnetNode::testBacnetNodeHandlerAnalogValueChangePresentValue(){
	bacnetNode->getAnalogObjectFromList(0)->setPresentValue(22.35);

	BACNET_ADDRESS dest;
	BACNET_ADDRESS my_address;
	BACNET_NPDU_DATA npdu_data;
	BACNET_READ_PROPERTY_DATA data;
	uint8_t invoke_id = 0;

	testBacnetNodeDriver->setupAddress(my_address, dest);
	// Configure Request
	data.object_type = OBJECT_ANALOG_VALUE;
	data.object_instance = (uint32_t)100;
	data.object_property = PROP_PRESENT_VALUE;
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
	ASSERT_EQUAL((float)22.35, appDataValueOUT.type.Real);
}
