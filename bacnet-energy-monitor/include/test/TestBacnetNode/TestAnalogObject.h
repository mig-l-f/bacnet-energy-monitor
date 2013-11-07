/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTANALOGOBJECT_H_
#define TESTANALOGOBJECT_H_

#include "cute.h"
#include "AnalogObject.h"
#include "AnalogValue.h"
#include "string.h"

class TestAnalogObject{
public:
	TestAnalogObject();
	~TestAnalogObject();

	//tests Analog Value Derived from Analog Object
	void nothing();
	void testSettingOfPresentValueIsCorrect();
	void testVerifyDescriptionChangeAndChangeToOriginal();
	void testInitialStatusFlags();
	void testInitialEventState();
	void testSetEventState();
	void testInitialReliability();
	void testSetReliability();
	void testIsOutOfService();
	void testUnits();
	void testWritePropertyReturnsFalseForDerivedAnalogValue();

	//tests Analog Output Derived from Analog Object

	//tests Analog Input Derived from Analog Object

private:
	AnalogObject* analog1;
	uint32_t objectNumber;
	const char* objectName;
	BACNET_ENGINEERING_UNITS units;
};

#endif /*TESTANALOGOBJECT_H_*/
