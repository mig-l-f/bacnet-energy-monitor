/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
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

void TestDeviceObject::testDeviceCountFunction(){
	ASSERT_EQUAL(MAX_BACNET_OBJECTS_PER_DEVICE, device->getCount());
}
void TestDeviceObject::testObject_Valid_Object_Instance_Number(){
	ASSERT_EQUAL(true, device->getValid_Object_Instance_Number((device->getObjectIdentifier())->instance));
}

// SUITE READ PROPERTY
void TestDeviceObject::createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
					BACNET_PROPERTY_ID property){

	rpdata.object_type = OBJECT_DEVICE;
	rpdata.object_instance = 235;
	rpdata.object_property = property;
	uint8_t apdu[480] = { 0 };
	rpdata.application_data = &apdu[0];
	appDataValueIN.context_specific = false;
	appDataValueIN.tag = BACNET_APPLICATION_TAG_OBJECT_ID;
	//appDataValueIN.type.Object_Id = *(device->getObjectIdentifier());
	BACNET_OBJECT_ID objID;
	objID.type = OBJECT_DEVICE;
	objID.instance = 235;
	appDataValueIN.type.Object_Id = objID;
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
	BACNET_CHARACTER_STRING name;
	characterstring_init_ansi(&name, "MyNewDevice");
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, &name));

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
	ASSERT_EQUAL(235, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(OBJECT_DEVICE, appDataValueOUT.type.Object_Id.type);
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
	ASSERT_EQUAL(OBJECT_DEVICE, appDataValueOUT.type.Object_Id.type);
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
	ASSERT_EQUAL(STATUS_NON_OPERATIONAL, appDataValueOUT.type.Enumerated);
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
	ASSERT_EQUAL(VENDOR_IDENTIFIER, appDataValueOUT.type.Unsigned_Int);
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
	BACNET_CHARACTER_STRING modelName;
	characterstring_init_ansi(&modelName, MODEL_NAME);
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
	BACNET_CHARACTER_STRING firmwareRev;
	characterstring_init_ansi(&firmwareRev, FIRMWARE_REV);
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
	BACNET_CHARACTER_STRING softwareRev;
	characterstring_init_ansi(&softwareRev, SOFTWARE_VERSION);
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
	ASSERT_EQUAL((uint32_t)BACNET_PROTOCOL_VERSION, appDataValueOUT.type.Unsigned_Int);
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
	ASSERT_EQUAL((uint32_t)BACNET_PROTOCOL_REVISION, appDataValueOUT.type.Unsigned_Int);
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
	BACNET_BIT_STRING protocolObjects;
	bitstring_init(&protocolObjects);
	for (int i = 0; i < MAX_ASHRAE_OBJECT_TYPE; i++) {
		bitstring_set_bit(&protocolObjects, (uint8_t) i, false);
	}
	bitstring_set_bit(&protocolObjects, OBJECT_DEVICE, true);
	ASSERT_EQUAL(true, bitstring_same(&protocolObjects, &appDataValueOUT.type.Bit_String)); //FIXME: No Bit_String
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

	//BACNET_OBJECT_ID* objectList = device->getObjectList();
	//int count = sizeof(objectList)/sizeof(BACNET_OBJECT_ID);
	int count = 1;

	for (int i = 0; i < count; i++){
		ASSERT_EQUAL(235, appDataValueOUT.type.Object_Id.instance);
		ASSERT_EQUAL(OBJECT_DEVICE, appDataValueOUT.type.Object_Id.type);
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

	//BACNET_OBJECT_ID* objectList = device->getObjectList();
	ASSERT_EQUAL(235, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(OBJECT_DEVICE, appDataValueOUT.type.Object_Id.type);
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
	ASSERT_EQUAL(MAX_APDU, appDataValueOUT.type.Unsigned_Int);
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
	ASSERT_EQUAL(SEGMENTATION_NONE, appDataValueOUT.type.Enumerated);
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
	ASSERT_EQUAL(apdu_timeout(), appDataValueOUT.type.Unsigned_Int);
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
	ASSERT_EQUAL(3,appDataValueOUT.type.Unsigned_Int);
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
	ASSERT_EQUAL(235, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(OBJECT_DEVICE, appDataValueOUT.type.Object_Id.type);
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
	ASSERT_EQUAL(0, appDataValueOUT.type.Unsigned_Int);
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
