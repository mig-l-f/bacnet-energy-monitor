/*
 * TestBacnetNode.h
 *
 *  Created on: 22 de Set de 2013
 *      Author: mgf
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
	void testBacnetNodeHandlerReadObjectName();
	void testBacnetNodeHandlerReadSystemStatus();
	void testBacnetNodeHandlerReadServicesSupported();
	void testBacnetNodeHandlerReadIndefiniteDeviceObjectInstance();
	void testBacnetNodeHandlerReadReject();
	void testBacnetNodeHandlerWhoIsWithLimits();
	void testBacnetNodeHandlerWhoIsWithWildcard();
	void testBacnetNodeHandlerWhoIsWithNoLimits();

	void testBacnetNodeHandlerAnalogValueObjectName();
	void testBacnetNodeHandlerAnalogValueReadPresentValue();
	void testBacnetNodeHandlerDeviceObjectListOfObjects();
	void testBacnetNodeHandlerAnalogValueChangePresentValue();
private:
	BacnetNode* bacnetNode;
	TestBacnetNodeDriver* testBacnetNodeDriver;
	TestBacnetNodeStub* testBacnetNodeStub;
};


#endif /* TESTBACNETNODE_H_ */
