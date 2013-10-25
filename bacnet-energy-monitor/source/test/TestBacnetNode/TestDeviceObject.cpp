/*
 * TestDeviceObject.cpp
 *
 *  Created on: 19 de Set de 2013
 *      Author: mgf
 */

#include "TestDeviceObject.h"

TestDeviceObject::TestDeviceObject(){
	deviceNumber = 235;
	deviceName = "MyNewDevice";
	deviceStatus = STATUS_NON_OPERATIONAL;
	device = new DeviceObject(deviceNumber, deviceName, deviceStatus);
}

TestDeviceObject::~TestDeviceObject(){
	delete(device);
}

void TestDeviceObject::nothing(){
	ASSERT_EQUAL(true, true);
}

void TestDeviceObject::testDeviceObjectIdentifier(){
	ASSERT_EQUAL(OBJECT_DEVICE, (device->getObjectIdentifier())->type);
	ASSERT_EQUAL(deviceNumber, device->getObjectIdentifier()->instance);
}

void TestDeviceObject::testDeviceObjectName(){
	BACNET_CHARACTER_STRING objectName;
	characterstring_init_ansi(&objectName, deviceName);

	BACNET_CHARACTER_STRING * temp = device->getObjectName();
	ASSERT_EQUAL(true, characterstring_same(temp,&objectName));
}
void TestDeviceObject::testDeviceObjectType(){
	ASSERT_EQUAL(OBJECT_DEVICE, device->getObjectType());
}
void TestDeviceObject::testSystemStatus(){
	ASSERT_EQUAL(deviceStatus, device->getDeviceStatus());
}
void TestDeviceObject::testVendorIDs(){
	BACNET_CHARACTER_STRING vendorID = device->getVendorName();
	ASSERT_EQUAL(true, characterstring_ansi_same(&vendorID,VENDOR_NAME));
	ASSERT_EQUAL(VENDOR_IDENTIFIER, device->getVendorIdentifier());
}
void TestDeviceObject::testModelName(){
	BACNET_CHARACTER_STRING modelName = device->getModelName();
	ASSERT_EQUAL(true, characterstring_ansi_same(&modelName, MODEL_NAME));
}
void TestDeviceObject::testFirmwareSoftwareRevision(){
	BACNET_CHARACTER_STRING firmwareRevision = device->getFirmwareRevision();
	ASSERT_EQUAL(true, characterstring_ansi_same(&firmwareRevision, FIRMWARE_REV));
	BACNET_CHARACTER_STRING softwareRevision = device->getSoftwareRevision();
	ASSERT_EQUAL(true, characterstring_ansi_same(&softwareRevision, SOFTWARE_VERSION));
}
void TestDeviceObject::testBacnetProtocolRevision(){
	ASSERT_EQUAL(BACNET_PROTOCOL_VERSION, device->getProtocolVersion());
	ASSERT_EQUAL(BACNET_PROTOCOL_REVISION, device->getProtocolRevision());
}
void TestDeviceObject::testProtocolObjectTypesSupported(){
	BACNET_BIT_STRING bitstring1;
	bitstring_init(&bitstring1);
	/*Initialize all objects to not supported*/
	for (int i = 0; i < MAX_ASHRAE_OBJECT_TYPE; i++)
		bitstring_set_bit(&bitstring1, (uint8_t) i, false);

	/*Initialize OBJECT_DEVICE only*/
	bitstring_set_bit(&bitstring1, (uint8_t) OBJECT_DEVICE, true);
	BACNET_BIT_STRING* protocolObject = device->getProtocolObjectTypesSupported();
	ASSERT_EQUAL(true, bitstring_same(&bitstring1, protocolObject));
}
void TestDeviceObject::testObjectList(){
	BACNET_OBJECT_ID objectID;
	objectID.type = OBJECT_DEVICE;
	objectID.instance = deviceNumber;

	BACNET_OBJECT_ID* objectList = device->getObjectList();
	ASSERT_EQUAL(objectID.instance, objectList[0].instance);
}
void TestDeviceObject::testMaxAPDULength(){
	ASSERT_EQUAL(MAX_APDU, device->getMaxApduLengthAccepted());
}
void TestDeviceObject::testSegmentationSupported(){
	ASSERT_EQUAL(SEGMENTATION_NONE, device->getSegmentationSupported());
}
void TestDeviceObject::testApduTimeout(){
	ASSERT_EQUAL(apdu_timeout(), device->getApduTimeout());
}
void TestDeviceObject::testNumberOfApduRetries(){
	ASSERT_EQUAL(apdu_retries(), device->getNumberOfApduRetries());
}
void TestDeviceObject::testDatabaseRevision(){
	ASSERT_EQUAL(0, device->getDatabaseRevision());
}
void TestDeviceObject::testDeviceCountFunction(){
	ASSERT_EQUAL(MAX_BACNET_OBJECTS_PER_DEVICE, device->getCount());
}
void TestDeviceObject::testObject_Valid_Object_Instance_Number(){
	ASSERT_EQUAL(true, device->getValid_Object_Instance_Number((device->getObjectIdentifier())->instance));
}

// SUITE READ PROPERTY
void TestDeviceObject::createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
					BACNET_PROPERTY_ID property){

	rpdata.object_type = device->getObjectType();
	rpdata.object_instance = (device->getObjectIdentifier())->instance;
	rpdata.object_property = property;
	uint8_t apdu[480] = { 0 };
	rpdata.application_data = &apdu[0];
	appDataValueIN.context_specific = false;
	appDataValueIN.tag = BACNET_APPLICATION_TAG_OBJECT_ID;
	appDataValueIN.type.Object_Id = *(device->getObjectIdentifier());
	rpdata.application_data_len = bacapp_encode_application_data(rpdata.application_data, &appDataValueIN);
}
void TestDeviceObject::testReadPropertyObjectName(){

	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_NAME);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, device->getObjectName()));

}
void TestDeviceObject::testReadPropertyObjectID(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_IDENTIFIER);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getObjectIdentifier()->instance, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(device->getObjectIdentifier()->type, appDataValueOUT.type.Object_Id.type);
}
void TestDeviceObject::testReadPropertyObjectType(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_TYPE);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getObjectType(), appDataValueOUT.type.Object_Id.type);
}
void TestDeviceObject::testReadPropertySystemStatus(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_SYSTEM_STATUS);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getDeviceStatus(), appDataValueOUT.type.Enumerated);
}
void TestDeviceObject::testReadPropertyVendorName(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_VENDOR_NAME);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	BACNET_CHARACTER_STRING vendorName;
	characterstring_init_ansi(&vendorName, VENDOR_NAME);
	ASSERT_EQUAL(true, characterstring_same(&vendorName, &appDataValueOUT.type.Character_String));
}
void TestDeviceObject::testReadPropertyVendorID(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_VENDOR_IDENTIFIER);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getVendorIdentifier(), appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertyModelName(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_MODEL_NAME);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	BACNET_CHARACTER_STRING modelName = device->getModelName();
	ASSERT_EQUAL(true, characterstring_same(&modelName, &appDataValueOUT.type.Character_String));
}
void TestDeviceObject::testReadPropertyFirmwareRevision(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_FIRMWARE_REVISION);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	BACNET_CHARACTER_STRING firmwareRev = device->getFirmwareRevision();
	ASSERT_EQUAL(true, characterstring_same(&firmwareRev, &appDataValueOUT.type.Character_String));
}
void TestDeviceObject::testReadPropertyApplicationSoftwareVersion(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_APPLICATION_SOFTWARE_VERSION);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	BACNET_CHARACTER_STRING softwareRev = device->getSoftwareRevision();
	ASSERT_EQUAL(true, characterstring_same(&softwareRev, &appDataValueOUT.type.Character_String));
}
void TestDeviceObject::testReadPropertyProtocolVersion(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_PROTOCOL_VERSION);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL((uint32_t)device->getProtocolVersion(), appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertyProtocolRevision(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_PROTOCOL_REVISION);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL((uint32_t)device->getProtocolRevision(), appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertyObjectTypesSupported(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_PROTOCOL_OBJECT_TYPES_SUPPORTED);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	BACNET_BIT_STRING* protocolObjects = device->getProtocolObjectTypesSupported();
	ASSERT_EQUAL(true, bitstring_same(protocolObjects, &appDataValueOUT.type.Bit_String)); //FIXME: No Bit_String
}
void TestDeviceObject::testReadPropertyObjectListCount(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	rpdata.array_index = 0;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_LIST);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);

	ASSERT_EQUAL(device->getCount(), appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertyObjectListALL(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	rpdata.array_index = BACNET_ARRAY_ALL;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_LIST);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
			ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);

	BACNET_OBJECT_ID* objectList = device->getObjectList();
	int count = sizeof(objectList)/sizeof(BACNET_OBJECT_ID);

	for (int i = 0; i < count; i++){
		ASSERT_EQUAL(objectList[i].instance, appDataValueOUT.type.Object_Id.instance);
		ASSERT_EQUAL(objectList[i].type, appDataValueOUT.type.Object_Id.type);
		if (appDataValueOUT.next != NULL)
			appDataValueOUT = *(appDataValueOUT.next);
	}
}
void TestDeviceObject::testReadPropertyObjectListIndexExists(){
	uint32_t idObject = 1;
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	rpdata.array_index = idObject;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_LIST);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
		ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);

	BACNET_OBJECT_ID* objectList = device->getObjectList();
	ASSERT_EQUAL(objectList[idObject-1].instance, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(objectList[idObject-1].type, appDataValueOUT.type.Object_Id.type);
}
void TestDeviceObject::testReadPropertyObjectListIndexNonExists(){
	uint32_t idObject = 100;
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	rpdata.array_index = idObject;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_LIST);

	int apdu_len = device->Object_Read_Property(&rpdata);
	ASSERT_EQUAL(BACNET_STATUS_ERROR, apdu_len);
}
void TestDeviceObject::testReadPropertyMaxAPDUlength(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_MAX_APDU_LENGTH_ACCEPTED);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
		ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getMaxApduLengthAccepted(), appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertySegmentationSupported(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_SEGMENTATION_SUPPORTED);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
		ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getSegmentationSupported(), appDataValueOUT.type.Enumerated);
}
void TestDeviceObject::testReadPropertyAPDUTimeout(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_APDU_TIMEOUT);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
		ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getApduTimeout(), appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertyAPDURetries(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_NUMBER_OF_APDU_RETRIES);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
		ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getNumberOfApduRetries(),appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertyDeviceAddressBinding(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_DEVICE_ADDRESS_BINDING);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
		ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	// Lista de BacnetAddressBinding são pares ObjectID + BACNET_ADDRESS
	// Vem codificados nos seguintes campos:
	// appDataValueOUT.type.Object_Id     ->  identificacao do objecto
	// appDataValueOUT.type.Unsigned_Int  ->  Bacnet Network Number
	// appDataValueOUT.type.Octet_String  ->  MAC address
	//FIXME So estamos a testar o Object_ID - listas so devem ser preenchidas dps de implementar servicos I-Am
	ASSERT_EQUAL(device->getObjectIdentifier()->instance, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(device->getObjectIdentifier()->type, appDataValueOUT.type.Object_Id.type);
}
void TestDeviceObject::testReadPropertyDatabaseRevision(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_DATABASE_REVISION);

	int apdu_len = device->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
		ASSERT_EQUAL(true, false);

	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, rpdata.application_data_len, &appDataValueOUT);
	ASSERT_EQUAL(device->getDatabaseRevision(), appDataValueOUT.type.Unsigned_Int);
}
void TestDeviceObject::testReadPropertyUnimplementedProperty(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_ACCESS_EVENT);
	int apdu_len = device->Object_Read_Property(&rpdata);
	ASSERT_EQUAL(BACNET_STATUS_ERROR, apdu_len);
	ASSERT_EQUAL(ERROR_CLASS_PROPERTY, rpdata.error_class);
	ASSERT_EQUAL(ERROR_CODE_UNKNOWN_PROPERTY, rpdata.error_code);
}
