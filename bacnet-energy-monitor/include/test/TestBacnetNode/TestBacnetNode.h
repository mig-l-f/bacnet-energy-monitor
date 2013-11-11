/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTBACNETNODE_H_
#define TESTBACNETNODE_H_

#include "cute.h"
#include "BacnetNode.h"
#include "TestBacnetNodeDriver.h"
#include "TestBacnetNodeStub.h"

class TestBacnetNode {
public:
	TestBacnetNode();
	~TestBacnetNode();

	// Tests
	void nothing();
	void testBacnetNodeHasDeviceObject();
	void testBacnetNodeHandlerWhoIsWithLimits();
	void testBacnetNodeHandlerWhoIsWithWildcard();
	void testBacnetNodeHandlerWhoIsWithNoLimits();

private:
	BacnetNode* bacnetNode;
	TestBacnetNodeDriver* testBacnetNodeDriver;
	TestBacnetNodeStub* testBacnetNodeStub;
};


#endif /* TESTBACNETNODE_H_ */
