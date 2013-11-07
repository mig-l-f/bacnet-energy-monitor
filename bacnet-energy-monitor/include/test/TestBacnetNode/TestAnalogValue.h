/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTANALOGVALUE_H_
#define TESTANALOGVALUE_H_

#include "cute.h"
#include "AnalogValue.h"
#include "bacdef.h"
#include "stdio.h"
#include "rp.h"
#include "bacapp.h"
#include "wp.h"

class TestAnalogValue{
public:
	TestAnalogValue();
	~TestAnalogValue();

	//tests
	void nothing();
	void testObjectIdentifier();
	void testObjectName();
	void testPresentValueIsNegativeOnCreation();
	void testDescriptionIsNoDescriptionByDefault();
	void testDescriptionCanBeChanged();
	void testStatusFlagsAreAllFalseOnCreation();
	void testEventStateHasValueNormalOnCreation();
	void testChangeEventStateToFaultAndBackToNormalHasConsistentStatusFlags();
	void testReliabilityHasNoFaultDetectedOnCreation();
	void testSettingReliabilityValuesHasOnlyFourOptions();
	void testChangeReliabilityFromNoFaultAndBackHasConsistentStatusFlags();
	void testOutOfServiceIsFalseOnCreation();
	void testUnitsIsDegreesCelsius();
	void testChangePresentValue();

	void testObjectCount();
	void testObject_Valid_Object_Instance_Number();
	void testReadPropertyObjectName();
	void testReadPropertyObjectID();
	void testReadPropertyObjectType();
	void testReadPresentValue();
	void testReadDescription();
	void testReadStatusFlags();
	void testReadEventState();
	void testReadReliability();
	void testReadOutOfService();
	void testReadUnits();
	void testReadChangedPresentValue();
	void testWritePropertyFails();

private:
	AnalogObject* analogValue;
	uint32_t objectNumber;
	const char* objectName;
	BACNET_ENGINEERING_UNITS units;
	void createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
						BACNET_PROPERTY_ID property);
	int wp_decode_apdu(uint8_t * apdu, unsigned apdu_len, uint8_t * invoke_id, BACNET_WRITE_PROPERTY_DATA * wpdata);


};
#endif /* TESTANALOGVALUE_H_ */
