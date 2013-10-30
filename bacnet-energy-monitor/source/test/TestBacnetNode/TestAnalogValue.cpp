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
	analogValue = new AnalogValue(objectNumber, objectName, "NOTDEF", units);
}

TestAnalogValue::~TestAnalogValue() {
	delete(analogValue);
}

void TestAnalogValue::nothing() {
	ASSERT_EQUAL(true, true);
}

void TestAnalogValue::testObjectIdentifier(){
	ASSERT_EQUAL(OBJECT_ANALOG_VALUE, (analogValue->getObjectIdentifier())->type);
	ASSERT_EQUAL(objectNumber, (analogValue->getObjectIdentifier())->instance);
}

void TestAnalogValue::testObjectName(){
	BACNET_CHARACTER_STRING analogValueName;
	characterstring_init_ansi(&analogValueName, objectName);

	BACNET_CHARACTER_STRING * temp = analogValue->getObjectName();
	ASSERT_EQUAL(true, characterstring_same(&analogValueName, temp));
}

void TestAnalogValue::testPresentValueIsNegativeOnCreation(){
	ASSERT_EQUAL((float)-1.0, analogValue->getPresentValue());
}

void TestAnalogValue::testDescriptionIsNoDescriptionByDefault(){
	std::string s = analogValue->getDescription();
	ASSERT_EQUAL(0, s.compare("NOTDEF"));
}
void TestAnalogValue::testDescriptionCanBeChanged(){
	ASSERT(analogValue->setDescription("MyNewDescription"));
	std::string s = analogValue->getDescription();
	ASSERT_EQUAL(0, s.compare("MyNewDescription"));
}

void TestAnalogValue::testStatusFlagsAreAllFalseOnCreation(){
	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);

	BACNET_BIT_STRING currentValues = analogValue->getStatusFlags();
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &currentValues));
}

void TestAnalogValue::testEventStateHasValueNormalOnCreation(){
	ASSERT_EQUAL(EVENT_STATE_NORMAL, analogValue->getEventState());
}

void TestAnalogValue::testChangeEventStateToFaultAndBackToNormalHasConsistentStatusFlags(){
	analogValue->setEventState(EVENT_STATE_FAULT);
	ASSERT_EQUAL(EVENT_STATE_FAULT, analogValue->getEventState());

	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, true);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);

	BACNET_BIT_STRING currentValues = analogValue->getStatusFlags();
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &currentValues));

	analogValue->setEventState(EVENT_STATE_NORMAL);
	ASSERT_EQUAL(EVENT_STATE_NORMAL, analogValue->getEventState());

	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	currentValues = analogValue->getStatusFlags();
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &currentValues));
}

void TestAnalogValue::testReliabilityHasNoFaultDetectedOnCreation(){
	ASSERT_EQUAL(RELIABILITY_NO_FAULT_DETECTED, analogValue->getReliability());
}

void TestAnalogValue::testSettingReliabilityValuesHasOnlyFourOptions(){
	ASSERT(analogValue->setReliability(RELIABILITY_NO_FAULT_DETECTED));
	ASSERT(analogValue->setReliability(RELIABILITY_OVER_RANGE));
	ASSERT(analogValue->setReliability(RELIABILITY_UNDER_RANGE));
	ASSERT(analogValue->setReliability(RELIABILITY_UNRELIABLE_OTHER));
	ASSERT_EQUAL(false, analogValue->setReliability(RELIABILITY_COMMUNICATION_FAILURE));
	ASSERT_EQUAL(false, analogValue->setReliability(RELIABILITY_NO_OUTPUT));
	ASSERT_EQUAL(false, analogValue->setReliability(RELIABILITY_OPEN_LOOP));
}

void TestAnalogValue::testChangeReliabilityFromNoFaultAndBackHasConsistentStatusFlags(){
	ASSERT(analogValue->setReliability(RELIABILITY_OVER_RANGE));

	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, true);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);
	BACNET_BIT_STRING currentValues = analogValue->getStatusFlags();
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &currentValues));

	ASSERT(analogValue->setReliability(RELIABILITY_NO_FAULT_DETECTED));
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, false);
	currentValues = analogValue->getStatusFlags();
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &currentValues));
}
void TestAnalogValue::testOutOfServiceIsFalseOnCreation(){
	ASSERT(!analogValue->isOutOfService());
}
void TestAnalogValue::testUnitsIsDegreesCelsius(){
	BACNET_ENGINEERING_UNITS currentUnits = analogValue->getUnits();
	if (currentUnits == units){
		ASSERT(true);
	}else{
		ASSERT(false);
	}
}
void TestAnalogValue::testChangePresentValue(){
	analogValue->setPresentValue(10.0);
	ASSERT_EQUAL(10.0, analogValue->getPresentValue());
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
	rpdata.object_type = analogValue->getObjectType();
	rpdata.object_instance = (analogValue->getObjectIdentifier())->instance;
	rpdata.object_property = property;
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
	ASSERT_EQUAL(true, characterstring_same(&appDataValueOUT.type.Character_String, analogValue->getObjectName()));
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
	ASSERT_EQUAL(analogValue->getObjectType(), appDataValueOUT.type.Object_Id.type);
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
	ASSERT_EQUAL(analogValue->getPresentValue(), appDataValueOUT.type.Real);
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
	characterstring_init_ansi(&desc, analogValue->getDescription());
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
	ASSERT_EQUAL(analogValue->getEventState(), appDataValueOUT.type.Enumerated);
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
	ASSERT_EQUAL(analogValue->getReliability(), appDataValueOUT.type.Enumerated);
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
	ASSERT_EQUAL(analogValue->isOutOfService(), appDataValueOUT.type.Boolean);
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
	ASSERT_EQUAL(analogValue->getUnits(), appDataValueOUT.type.Enumerated);
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
	ASSERT_EQUAL(analogValue->getPresentValue(), appDataValueOUT.type.Real);

	analogValue->setPresentValue(35.0);

	createAPDU(rpdata, appDataValueIN, PROP_PRESENT_VALUE);

	apdu_len = analogValue->Object_Read_Property(&rpdata);
	if (apdu_len == BACNET_STATUS_ERROR)
				ASSERT_EQUAL(true, false);
	bacapp_decode_application_data(rpdata.application_data, MAX_APDU, &appDataValueOUT);
	ASSERT_EQUAL(analogValue->getPresentValue(), appDataValueOUT.type.Real);
}
