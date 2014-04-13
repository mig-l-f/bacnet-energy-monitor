/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTANALOGVALUENODE_H_
#define TESTANALOGVALUENODE_H_

#include "cute.h"
#include "AnalogValueNode.h"
#include "TestBacnetNodeDriver.h"
#include "TestBacnetNodeStub.h"

class TestAnalogValueNode{
public:
	TestAnalogValueNode();
	~TestAnalogValueNode();

	void nothing();
	void testNodeHandlerReadPropertyObjectName();
	void testNodeHandlerReadProperyPresentValue();
	void testNodeHandlerReadAccessNonExistingObject();
	void testNodeHandlerReadNonExistingProperty();
	void testNodeReadPresentValueAfterInserting1Value();

private:
	AnalogValueNode* bacnetNode;
	TestBacnetNodeDriver* testBacnetNodeDriver;
	TestBacnetNodeStub* testBacnetNodeStub;
};

#endif
