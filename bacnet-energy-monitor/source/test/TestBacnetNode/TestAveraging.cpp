/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestAveraging.h"

TestAveraging::TestAveraging(){
	averaging = new Averaging(100, "MyAveragingObject");
}

TestAveraging::~TestAveraging(){
	delete(averaging);
}

void TestAveraging::nothing(){}

void TestAveraging::testCount(){
	ASSERT_EQUAL(1, averaging->getCount());
}

void TestAveraging::testObject_Valid_Object_Instance_Number(){
//	ASSERT_EQUAL(true, averaging->getValid_Object_Instance_Number(averaging->getObjectIdentifier()->instance));
	ASSERT_EQUAL(true, averaging->getValid_Object_Instance_Number(100));
//	ASSERT_EQUAL(false, averaging->getValid_Object_Instance_Number(33));
}

void TestAveraging::createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
					BACNET_PROPERTY_ID property){
//	rpdata.object_type = OBJECT_AVERAGING;
//	rpdata.object_instance = (averaging->getObjectIdentifier())->instance;
	rpdata.object_property = property;
	rpdata.array_index = BACNET_ARRAY_ALL;
	uint8_t apdu[1000] = { 0 };
	rpdata.application_data = &apdu[0];
	appDataValueIN.context_specific = false;
	appDataValueIN.tag = BACNET_APPLICATION_TAG_OBJECT_ID;
	appDataValueIN.type.Object_Id = *(averaging->getObjectIdentifier());
	rpdata.application_data_len = bacapp_encode_application_data(rpdata.application_data, &appDataValueIN);
}

void TestAveraging::testReadPropertyObjectName(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_NAME);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	BACNET_CHARACTER_STRING name;
	characterstring_init_ansi(&name, "MyAveragingObject");
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, &name));
}

void TestAveraging::testReadPropertyObjectID(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_IDENTIFIER);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(averaging->getObjectIdentifier()->instance, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(averaging->getObjectIdentifier()->type, appDataValueOUT.type.Object_Id.type);
}

void TestAveraging::testReadPropertyObjectType(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_TYPE);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(OBJECT_AVERAGING, appDataValueOUT.type.Object_Id.type);
}

