/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTAVERAGINGNODE_H_
#define TESTAVERAGINGNODE_H_

#include "cute.h"
#include "AveragingNode.h"
#include "TestBacnetNodeDriver.h"
#include "TestBacnetNodeStub.h"

class TestAveragingNode{
public:
	TestAveragingNode();
	~TestAveragingNode();

// Tests:
	void nothing();
	void testAveragingNodeHandlerReadObjectName();
	void testAveragingNodeHandlerReadAverageValue();
	void testAveragingNodeHandlerReadAccessNonExistingObject();
	void testAveragingNodeHandlerReadNonExistingProperty();
	void testAveragingNodeHandlerSendingSegmentedMessage();
	void testAveragingReadAverageAfterInserting1Value();
	void testAveragingReadAverageMaximumMinimumAfterInserting50Values();
	void testAveragingReadAverageAfterCollectingSampleFromAnalogValue();

private:
	AveragingNode* bacnetNode;
	TestBacnetNodeDriver* testBacnetNodeDriver;
	TestBacnetNodeStub* testBacnetNodeStub;
};

#endif /* TESTAVERAGINGNODE_H_ */
