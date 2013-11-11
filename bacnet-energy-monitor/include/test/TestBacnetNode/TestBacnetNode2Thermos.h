/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTBACNETNODE2THERMOS_H_
#define TESTBACNETNODE2THERMOS_H_

#include "cute.h"
#include "BacnetNode2Thermos.h"
#include "TestBacnetNodeDriver.h"
#include "TestBacnetNodeStub.h"
#include "bacenum.h"

class TestBacnetNode2Thermos{
public:
	TestBacnetNode2Thermos();
	~TestBacnetNode2Thermos();

	// Tests
	void nothing();
	void testBacnetNode2ThermosHandlerReadObjectName();
	void testBacnetNode2ThermosHandlerReadSystemStatus();
	void testBacnetNode2ThermosHandlerReadServicesSupported();
	void testBacnetNode2ThermosHandlerReadIndefiniteDeviceObjectInstance();
	void testBacnetNode2ThermosHandlerReadReject();
	void testBacnetNode2ThermosHandlerWhoIsWithLimits();
	void testBacnetNode2ThermosHandlerWhoIsWithWildcard();
	void testBacnetNode2ThermosHandlerWhoIsWithNoLimits();

	void testBacnetNode2ThermosHandlerAnalogValueObjectName();
	void testBacnetNode2ThermosHandlerAnalogValueReadPresentValue();
	void testBacnetNode2ThermosHandlerDeviceObjectListOfObjects();
	void testBacnetNode2ThermosHandlerAnalogValueChangePresentValue();
	void testBacnetNode2ThermosReadAV2Object();
	void testBacnetNode2ThermosReadInvalidInstance();
	void testBacnetNode2ThermosReadAVWithArrayIndexFails();
private:
	BacnetNode2Thermos* bacnetNode;
	TestBacnetNodeDriver* testBacnetNodeDriver;
	TestBacnetNodeStub* testBacnetNodeStub;
};

#endif
