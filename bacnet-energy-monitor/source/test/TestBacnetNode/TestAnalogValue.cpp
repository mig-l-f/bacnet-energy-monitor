/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestAnalogValue.h"

TestAnalogValue::TestAnalogValue() {
	objectNumber = 100;
	objectName = "MyAnalogValue";
	units = UNITS_DEGREES_CELSIUS;
	analogValue = new AnalogValue(objectNumber, objectName, "ThisIsDescription", units, 50.0, -10.0);
}

TestAnalogValue::~TestAnalogValue() {
	delete(analogValue);
}

void TestAnalogValue::testObjectCount(){
	ASSERT_EQUAL(1, analogValue->getCount());
}
void TestAnalogValue::testObject_Valid_Object_Instance_Number(){
	ASSERT_EQUAL(true, analogValue->getValid_Object_Instance_Number(analogValue->getObjectIdentifier()->instance));
	ASSERT_EQUAL(true, analogValue->getValid_Object_Instance_Number(objectNumber));
	ASSERT_EQUAL(false, analogValue->getValid_Object_Instance_Number(objectNumber+5));
}
void TestAnalogValue::createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
					BACNET_PROPERTY_ID property){
	rpdata.object_type = OBJECT_ANALOG_VALUE;
	rpdata.object_instance = (analogValue->getObjectIdentifier())->instance;
	rpdata.object_property = property;
	rpdata.array_index = BACNET_ARRAY_ALL;
	uint8_t apdu[1000] = { 0 };
	rpdata.application_data = &apdu[0];
	appDataValueIN.context_specific = false;
	appDataValueIN.tag = BACNET_APPLICATION_TAG_OBJECT_ID;
	appDataValueIN.type.Object_Id = *(analogValue->getObjectIdentifier());
	rpdata.application_data_len = bacapp_encode_application_data(rpdata.application_data, &appDataValueIN);
}

void TestAnalogValue::testReadPropertyObjectName(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_NAME);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	BACNET_CHARACTER_STRING name;
	characterstring_init_ansi(&name, "MyAnalogValue");
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, &name));
}
void TestAnalogValue::testReadPropertyObjectID(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_IDENTIFIER);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(analogValue->getObjectIdentifier()->instance, appDataValueOUT.type.Object_Id.instance);
	ASSERT_EQUAL(analogValue->getObjectIdentifier()->type, appDataValueOUT.type.Object_Id.type);
}
void TestAnalogValue::testReadPropertyObjectType(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OBJECT_TYPE);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(OBJECT_ANALOG_VALUE, appDataValueOUT.type.Object_Id.type);
}
void TestAnalogValue::testReadPresentValue(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(-1.0, appDataValueOUT.type.Real);
}
void TestAnalogValue::testReadDescription(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_DESCRIPTION);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	BACNET_CHARACTER_STRING desc;
	characterstring_init_ansi(&desc, "ThisIsDescription");
	ASSERT_EQUAL(true, characterstring_same(&desc, &appDataValueOUT.type.Character_String));
}
void TestAnalogValue::testReadStatusFlags(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_STATUS_FLAGS);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);

	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &appDataValueOUT.type.Bit_String));
}
void TestAnalogValue::testReadEventState(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_EVENT_STATE);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(EVENT_STATE_NORMAL, appDataValueOUT.type.Enumerated);
}
void TestAnalogValue::testReadReliability(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_RELIABILITY);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(RELIABILITY_NO_FAULT_DETECTED, appDataValueOUT.type.Enumerated);
}
void TestAnalogValue::testReadOutOfService(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_OUT_OF_SERVICE);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(false, appDataValueOUT.type.Boolean);
}
void TestAnalogValue::testReadUnits(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_UNITS);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(UNITS_DEGREES_CELSIUS, appDataValueOUT.type.Enumerated);
}
void TestAnalogValue::testReadChangedPresentValue(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(-1.0, appDataValueOUT.type.Real);

	analogValue->setPresentValue(35.0);

	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);

	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(35.0, appDataValueOUT.type.Real);
}
int TestAnalogValue::wp_decode_apdu(uint8_t * apdu, unsigned apdu_len, uint8_t * invoke_id, BACNET_WRITE_PROPERTY_DATA * wpdata){
	int len = 0;
	unsigned offset = 0;

	if (!apdu)
		return -1;
	/* optional checking - most likely was already done prior to this call */
	if (apdu[0] != PDU_TYPE_CONFIRMED_SERVICE_REQUEST)
		return -1;
	/*  apdu[1] = encode_max_segs_max_apdu(0, MAX_APDU); */
	*invoke_id = apdu[2];       /* invoke id - filled in by net layer */
	if (apdu[3] != SERVICE_CONFIRMED_WRITE_PROPERTY)
		return -1;
	offset = 4;

	if (apdu_len > offset) {
		len = wp_decode_service_request(&apdu[offset], apdu_len - offset, wpdata);
	}

	return len;
}
void TestAnalogValue::testWritePropertyFails(){
	BACNET_WRITE_PROPERTY_DATA wpdata;
	BACNET_APPLICATION_DATA_VALUE input_value;
//	BACNET_WRITE_PROPERTY_DATA test_data;

//	uint8_t invoke_id = 128;
//	uint8_t test_invoke_id = 0;
//	int len = 0; int apdu_len = 0;
//	uint8_t apdu[1000] = { 0 };

	input_value.tag = BACNET_APPLICATION_TAG_BOOLEAN;
	input_value.type.Boolean = true;

	wpdata.application_data_len = bacapp_encode_application_data(&wpdata.application_data[0], &input_value);

	bool success = analogValue->Object_Write_Property(&wpdata);
	ASSERT_EQUAL(false, success);

//	len = wp_encode_apdu(&apdu[0], invoke_id, &wpdata);
//	ASSERT_EQUAL(true, len != 0);
//	apdu_len = len;
//	len = wp_decode_apdu(&apdu[0], apdu_len, &test_invoke_id, &test_data);
//	ASSERT_EQUAL(true, len != -1);
	ASSERT_EQUAL(ERROR_CLASS_PROPERTY, wpdata.error_class);
	ASSERT_EQUAL(ERROR_CODE_WRITE_ACCESS_DENIED, wpdata.error_code);
}

void TestAnalogValue::testReadHighLimit(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_HIGH_LIMIT);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(50.0, appDataValueOUT.type.Real);
}

void TestAnalogValue::testReadLowLimit(){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;
	createAPDU(rpdata, appDataValueIN, PROP_LOW_LIMIT);

	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(-10.0, appDataValueOUT.type.Real);
}

void TestAnalogValue::testSetPresentValueAboveLimitAndReadReliabilityAndStatus(){
	analogValue->setPresentValue(200.0);

	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;

	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);
	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(200.0, appDataValueOUT.type.Real);

	createAPDU(rpdata, appDataValueIN, PROP_RELIABILITY);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(RELIABILITY_OVER_RANGE, appDataValueOUT.type.Enumerated);

	createAPDU(rpdata, appDataValueIN, PROP_STATUS_FLAGS);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);

	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, true);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &appDataValueOUT.type.Bit_String));
}

void TestAnalogValue::testSetPresentValueBelowLimitAndReadReliabilityAndStatus(){
	analogValue->setPresentValue(-200.0);

	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;

	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);
	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(-200.0, appDataValueOUT.type.Real);

	createAPDU(rpdata, appDataValueIN, PROP_RELIABILITY);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(RELIABILITY_UNDER_RANGE, appDataValueOUT.type.Enumerated);

	createAPDU(rpdata, appDataValueIN, PROP_STATUS_FLAGS);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);

	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, true);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &appDataValueOUT.type.Bit_String));
}
void TestAnalogValue::testSetPresentValueOutsideLimitReadStatusResetPresentValueAndReadStatus(){
	analogValue->setPresentValue(-200.0);

	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_APPLICATION_DATA_VALUE appDataValueIN;

	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);
	int apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	BACNET_APPLICATION_DATA_VALUE appDataValueOUT;
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(-200.0, appDataValueOUT.type.Real);

	createAPDU(rpdata, appDataValueIN, PROP_RELIABILITY);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(RELIABILITY_UNDER_RANGE, appDataValueOUT.type.Enumerated);

	createAPDU(rpdata, appDataValueIN, PROP_STATUS_FLAGS);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);

	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, true);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &appDataValueOUT.type.Bit_String));

	analogValue->setPresentValue(25.0);

	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(25.0, appDataValueOUT.type.Real);

	createAPDU(rpdata, appDataValueIN, PROP_RELIABILITY);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(RELIABILITY_NO_FAULT_DETECTED, appDataValueOUT.type.Enumerated);

	createAPDU(rpdata, appDataValueIN, PROP_STATUS_FLAGS);
	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);

	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &appDataValueOUT.type.Bit_String));
}
