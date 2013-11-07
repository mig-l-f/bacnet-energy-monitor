/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestAnalogObject.h"

TestAnalogObject::TestAnalogObject(){
	objectNumber = 101;
	objectName = "TestAnalogObject";
	units = UNITS_DEGREES_CELSIUS;
	analog1 = new AnalogValue(objectNumber, objectName, "Test Analog Object", units);
}

TestAnalogObject::~TestAnalogObject(){}

void TestAnalogObject::nothing(){
	ASSERT_EQUAL(true, true);
}

void TestAnalogObject::testSettingOfPresentValueIsCorrect(){
	float value = (float)1012.342;
	analog1->setPresentValue(value);
	ASSERT_EQUAL(value, analog1->getPresentValue());
}

void TestAnalogObject::testVerifyDescriptionChangeAndChangeToOriginal(){
	std::string originalDescription = analog1->getDescription();
	ASSERT_EQUAL(0,originalDescription.compare("Test Analog Object"));

	analog1->setDescription("New Description");
	std::string currentDescription = analog1->getDescription();
	ASSERT_EQUAL(0, currentDescription.compare("New Description"));

	analog1->setDescription(originalDescription.c_str());
	ASSERT_EQUAL(0, originalDescription.compare(analog1->getDescription()));
}

void TestAnalogObject::testInitialStatusFlags(){
	BACNET_BIT_STRING referenceValues;
	bitstring_init(&referenceValues);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_FAULT, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&referenceValues, STATUS_FLAG_OVERRIDDEN, false);

	BACNET_BIT_STRING currentValues = analog1->getStatusFlags();
	ASSERT_EQUAL(true, bitstring_same(&referenceValues, &currentValues));
}
void TestAnalogObject::testInitialEventState(){
	ASSERT_EQUAL(EVENT_STATE_NORMAL, analog1->getEventState());
}
void TestAnalogObject::testSetEventState(){
	analog1->setEventState(EVENT_STATE_OFFNORMAL);
	ASSERT_EQUAL(EVENT_STATE_OFFNORMAL, analog1->getEventState());
}
void TestAnalogObject::testInitialReliability(){
	ASSERT_EQUAL(RELIABILITY_NO_FAULT_DETECTED, analog1->getReliability());
}
void TestAnalogObject::testSetReliability(){
	bool success = analog1->setReliability(RELIABILITY_OVER_RANGE);
	ASSERT_EQUAL(true, success);
	ASSERT_EQUAL(RELIABILITY_OVER_RANGE, analog1->getReliability());
}
void TestAnalogObject::testIsOutOfService(){
	ASSERT_EQUAL(false, analog1->isOutOfService());
}
void TestAnalogObject::testUnits(){
	BACNET_ENGINEERING_UNITS currentUnits = analog1->getUnits();
	if (currentUnits == UNITS_DEGREES_CELSIUS){
		ASSERT(true);
	}else{
		ASSERT(false);
	}
}
void TestAnalogObject::testWritePropertyReturnsFalseForDerivedAnalogValue(){
	BACNET_WRITE_PROPERTY_DATA wpdata;
	ASSERT_EQUAL(false, analog1->Object_Write_Property(&wpdata));
	ASSERT_EQUAL(ERROR_CLASS_PROPERTY, wpdata.error_class);
	ASSERT_EQUAL(ERROR_CODE_WRITE_ACCESS_DENIED, wpdata.error_code);
}
