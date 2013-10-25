/*
 * TestAnalogValue.h
 *
 *  Created on: 24 de Set de 2013
 *      Author: mgf
 */

#ifndef TESTANALOGVALUE_H_
#define TESTANALOGVALUE_H_

#include "cute.h"
#include "AnalogValue.h"
#include "bacdef.h"
#include "stdio.h"
#include "iostream"
#include "rp.h"
#include "bacapp.h"

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
private:
	AnalogValue* analogValue;
	uint32_t objectNumber;
	const char* objectName;
	BACNET_ENGINEERING_UNITS units;
	void createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
						BACNET_PROPERTY_ID property);
};
#endif /* TESTANALOGVALUE_H_ */
