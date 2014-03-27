/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestAveraging.h"

TestAveraging::TestAveraging(){
	averaging = new Averaging(100, "MyAveragingObject", 900, 45, 10, 20);
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

void TestAveraging::testReadPropertyMinimumValue(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_MINIMUM_VALUE);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, isinff(appDataValueOUT.type.Real) == 1 || isinff(appDataValueOUT.type.Real) == -1);
}

void TestAveraging::testReadPropertyMaximumValue(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_MAXIMUM_VALUE);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, isinff(appDataValueOUT.type.Real) == 1);
}

void TestAveraging::testReadPropertyAttemptedSamples(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_ATTEMPTED_SAMPLES);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(0, appDataValueOUT.type.Unsigned_Int);
}

void TestAveraging::testReadPropertyValidSamples(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_VALID_SAMPLES);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(0, appDataValueOUT.type.Unsigned_Int);
}

void TestAveraging::testReadPropertyAverageValue(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_AVERAGE_VALUE);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(true, isnanf(appDataValueOUT.type.Real) != 0);
}

void TestAveraging::testReadWindowInterval(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_WINDOW_INTERVAL);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(900, appDataValueOUT.type.Unsigned_Int);
}

void TestAveraging::testReadWindowSamples(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_WINDOW_SAMPLES);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(45, appDataValueOUT.type.Unsigned_Int);
}

void TestAveraging::testReadNonExistingProperty(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_CONFIGURATION_FILES);
	int apdu_len = averaging->Object_Read_Property(&rpdata);
	ASSERT_EQUAL(BACNET_STATUS_ERROR, apdu_len);
	ASSERT_EQUAL(ERROR_CLASS_PROPERTY, rpdata.error_class);
	ASSERT_EQUAL(ERROR_CODE_UNKNOWN_PROPERTY, rpdata.error_code);
}

void TestAveraging::testReadPropertyArrayIndexOfNonArrayObject(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_AVERAGE_VALUE);
	rpdata.array_index = 1;
	int apdu_len = averaging->Object_Read_Property(&rpdata);
	ASSERT_EQUAL(BACNET_STATUS_ERROR, apdu_len);
	ASSERT_EQUAL(ERROR_CLASS_PROPERTY, rpdata.error_class);
	ASSERT_EQUAL(ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY, rpdata.error_code);
}

void TestAveraging::testReadPropertyAverageValueAfterInsertingNewValue(){
	//Add Value
	float value = 15.0;
	averaging->addNewSample(value);

	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_AVERAGE_VALUE);

	int apdu_len = averaging->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(value, appDataValueOUT.type.Real);
}
