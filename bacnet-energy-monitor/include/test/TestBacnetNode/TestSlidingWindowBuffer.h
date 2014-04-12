/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTSLIDINGWINDOWBUFFER_H_
#define TESTSLIDINGWINDOWBUFFER_H_

#include "cute.h"
#include "SlidingWindowBuffer.h"
#include "math.h"

class TestSlidingWindowBuffer{
public:
	TestSlidingWindowBuffer();
	~TestSlidingWindowBuffer();

	void nothing();
	void testNumberOfValidSamples();
	void testWindowSizeValue();
	void testAverageValueWithNoSamples();
	void insert1ValidValueAndCalculateAverage();
	void insert3ValidValuesAndCalculateAverage();
	void insert4ValidValuesAndCalculateAverage();
	void insert30ValidValuesAndCalculateAverage();
	void insertInvalidValueAsFirstValue();
	void insertValidAndInvalidValue();
	void testMinimumValueIsInfBeforeAnySampleIsAdded();
	void insert5ValuesAndVerifyMinimum();
	void testMaximumIsInfBeforeAnySampleIsAdded();
	void insert10ValuesAndVerifyMaximum();
	void testNumberOfAttemptedSamples();
	void testNumberOfAttemptedSamplesAfterInsertingValidAndInvalidSamples();

private:
	SlidingWindowBuffer* buffer;
};

#endif /* TESTSLIDINGWINDOWBUFFER_H_ */
