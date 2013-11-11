/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include "TestDeviceObject.h"
#include "TestBacnetNode.h"
#include "TestAnalogValue.h"
#include "TestAnalogObject.h"
#include "TestBacnetNode2Thermos.h"


void runSuiteDeviceObjectProperties(){
	cute::suite s;
	s += CUTE_SMEMFUN(TestDeviceObject, testDeviceObjectIdentifier);
	s += CUTE_SMEMFUN(TestDeviceObject, testDeviceObjectName);
	s += CUTE_SMEMFUN(TestDeviceObject, testDeviceObjectType);
	s += CUTE_SMEMFUN(TestDeviceObject, testSystemStatus);
	s += CUTE_SMEMFUN(TestDeviceObject, testVendorIDs);
	s += CUTE_SMEMFUN(TestDeviceObject, testModelName);
	s += CUTE_SMEMFUN(TestDeviceObject, testFirmwareSoftwareRevision);
	s += CUTE_SMEMFUN(TestDeviceObject, testBacnetProtocolRevision);
	s += CUTE_SMEMFUN(TestDeviceObject, testProtocolObjectTypesSupported);
	s += CUTE_SMEMFUN(TestDeviceObject, testObjectList);
	s += CUTE_SMEMFUN(TestDeviceObject, testMaxAPDULength);
	s += CUTE_SMEMFUN(TestDeviceObject, testSegmentationSupported);
	s += CUTE_SMEMFUN(TestDeviceObject, testApduTimeout);
	s += CUTE_SMEMFUN(TestDeviceObject, testNumberOfApduRetries);
	s += CUTE_SMEMFUN(TestDeviceObject, testDatabaseRevision);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Suite: Device Object Properties");
}

void runSuiteDeviceObjectReadPropertyTest(){
	cute::suite s;

	s += CUTE_SMEMFUN(TestDeviceObject, testDeviceCountFunction);
	s += CUTE_SMEMFUN(TestDeviceObject, testObject_Valid_Object_Instance_Number);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectName);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectID);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectType);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertySystemStatus);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyVendorName);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyVendorID);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyModelName);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyFirmwareRevision);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyApplicationSoftwareVersion);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyProtocolVersion);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyProtocolRevision);
//	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectTypesSupported); //FIXME: Bug 23
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectListCount);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectListALL);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectListIndexExists);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyObjectListIndexNonExists);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyMaxAPDUlength);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertySegmentationSupported);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyAPDUTimeout);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyAPDURetries);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyDeviceAddressBinding);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyDatabaseRevision);
	s += CUTE_SMEMFUN(TestDeviceObject, testReadPropertyUnimplementedProperty);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Suite: Device Object Read Property Test");
}

void runSuiteBacnetNodeProperties(){
	cute::suite s;

	s += CUTE_SMEMFUN(TestBacnetNode, testBacnetNodeHasDeviceObject);
	s += CUTE_SMEMFUN(TestBacnetNode, testBacnetNodeHandlerWhoIsWithLimits);
	s += CUTE_SMEMFUN(TestBacnetNode, testBacnetNodeHandlerWhoIsWithWildcard);
	s += CUTE_SMEMFUN(TestBacnetNode, testBacnetNodeHandlerWhoIsWithNoLimits);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Suite: Bacnet Node Properties");
}

void runSuiteAnalogValueObjectProperties(){
	cute::suite s;

	s += CUTE_SMEMFUN(TestAnalogValue, nothing);
	s += CUTE_SMEMFUN(TestAnalogValue, testObjectIdentifier);
	s += CUTE_SMEMFUN(TestAnalogValue, testObjectName);
	s += CUTE_SMEMFUN(TestAnalogValue, testPresentValueIsNegativeOnCreation);
	s += CUTE_SMEMFUN(TestAnalogValue, testDescriptionIsNoDescriptionByDefault);
	s += CUTE_SMEMFUN(TestAnalogValue, testDescriptionCanBeChanged);
	s += CUTE_SMEMFUN(TestAnalogValue, testStatusFlagsAreAllFalseOnCreation);
	s += CUTE_SMEMFUN(TestAnalogValue, testEventStateHasValueNormalOnCreation);
	s += CUTE_SMEMFUN(TestAnalogValue, testChangeEventStateToFaultAndBackToNormalHasConsistentStatusFlags);
	s += CUTE_SMEMFUN(TestAnalogValue, testReliabilityHasNoFaultDetectedOnCreation);
	s += CUTE_SMEMFUN(TestAnalogValue, testSettingReliabilityValuesHasOnlyFourOptions);
	s += CUTE_SMEMFUN(TestAnalogValue, testChangeReliabilityFromNoFaultAndBackHasConsistentStatusFlags);
	s += CUTE_SMEMFUN(TestAnalogValue, testOutOfServiceIsFalseOnCreation);
	s += CUTE_SMEMFUN(TestAnalogValue, testUnitsIsDegreesCelsius);
	s += CUTE_SMEMFUN(TestAnalogValue, testChangePresentValue);
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Suite: Analog Value Object Properties");
}

void runSuiteAnalogValueReadPropertyTest(){
	cute::suite s;
	cute::ide_listener lis;
	s += CUTE_SMEMFUN(TestAnalogValue, testObjectCount);
	s += CUTE_SMEMFUN(TestAnalogValue, testObject_Valid_Object_Instance_Number);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadPropertyObjectName);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadPropertyObjectID);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadPropertyObjectType);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadPresentValue);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadDescription);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadStatusFlags);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadEventState);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadReliability);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadOutOfService);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadUnits);
	s += CUTE_SMEMFUN(TestAnalogValue, testReadChangedPresentValue);
	s += CUTE_SMEMFUN(TestAnalogValue, testWritePropertyFails);
	cute::makeRunner(lis)(s, "Suite: Analog Value Read Properties");
}

void runSuitAnalogObject(){
	cute::suite s;
	s += CUTE_SMEMFUN(TestAnalogObject, testSettingOfPresentValueIsCorrect);
	s += CUTE_SMEMFUN(TestAnalogObject, testVerifyDescriptionChangeAndChangeToOriginal);
	s += CUTE_SMEMFUN(TestAnalogObject, testInitialStatusFlags);
	s += CUTE_SMEMFUN(TestAnalogObject, testInitialEventState);
	s += CUTE_SMEMFUN(TestAnalogObject, testSetEventState);
	s += CUTE_SMEMFUN(TestAnalogObject, testInitialReliability);
	s += CUTE_SMEMFUN(TestAnalogObject, testSetReliability);
	s += CUTE_SMEMFUN(TestAnalogObject, testIsOutOfService);
	s += CUTE_SMEMFUN(TestAnalogObject, testUnits);
	s += CUTE_SMEMFUN(TestAnalogObject, testWritePropertyReturnsFalseForDerivedAnalogValue);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Suite: Test Analog Object");
}

void runSuitBacnetNode2Thermos(){
	cute::suite s;
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerReadObjectName);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerReadSystemStatus);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerReadServicesSupported);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerReadIndefiniteDeviceObjectInstance);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerReadReject);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerWhoIsWithLimits);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerWhoIsWithWildcard);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerWhoIsWithNoLimits);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerAnalogValueObjectName);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerAnalogValueReadPresentValue);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerDeviceObjectListOfObjects);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosHandlerAnalogValueChangePresentValue);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosReadAV2Object);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosReadInvalidInstance);
	s += CUTE_SMEMFUN(TestBacnetNode2Thermos, testBacnetNode2ThermosReadAVWithArrayIndexFails);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Suite: Test Bacnet Node 2 Thermos");
}

int main(){
    runSuiteDeviceObjectProperties();
    runSuiteDeviceObjectReadPropertyTest();
    runSuiteBacnetNodeProperties();
    runSuiteAnalogValueObjectProperties();
    runSuiteAnalogValueReadPropertyTest();
    runSuitAnalogObject();
    runSuitBacnetNode2Thermos();
    return 0;
}



