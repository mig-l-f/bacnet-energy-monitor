/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "TestSlidingWindowBuffer.h"

TestSlidingWindowBuffer::TestSlidingWindowBuffer(){
	int bufferSize = 3;
	buffer = new SlidingWindowBuffer(bufferSize, 0.0, 100.0);
}

TestSlidingWindowBuffer::~TestSlidingWindowBuffer(){
	delete(buffer);
}

void TestSlidingWindowBuffer::nothing(){
	ASSERT(true);
}

void TestSlidingWindowBuffer::testNumberOfValidSamples(){
	ASSERT_EQUAL(0, buffer->getNumberOfValidSamples());
}

void TestSlidingWindowBuffer::testWindowSizeValue(){
	ASSERT_EQUAL(3, buffer->getWindowSize());
}
void TestSlidingWindowBuffer::testAverageValueWithNoSamples(){
	ASSERT_EQUAL(true, isnanf(buffer->getAverage()) != 0);
}
void TestSlidingWindowBuffer::insert1ValidValueAndCalculateAverage(){
	float sample = 5.0;
	buffer->addSample(sample);
	ASSERT_EQUAL(1, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(true, isnanf(buffer->getAverage()) == 0);
    ASSERT_EQUAL(sample, buffer->getAverage());
}

void TestSlidingWindowBuffer::insert3ValidValuesAndCalculateAverage(){
	float sample = 5.0;
	buffer->addSample(sample);
	sample += 1.0;
	buffer->addSample(sample);
	sample += 1.0;
	buffer->addSample(sample);

	ASSERT_EQUAL(3, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(6.0, buffer->getAverage());
}

void TestSlidingWindowBuffer::insert4ValidValuesAndCalculateAverage(){
	float sample = 10.0;
	for(int i = 0; i < 4; i++){
		buffer->addSample(sample);
		sample += 1.0;
	}
	ASSERT_EQUAL(3, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(12.0, buffer->getAverage());
}

void TestSlidingWindowBuffer::insert30ValidValuesAndCalculateAverage(){
	float sample = 1.0;
	for (int i = 0; i < 30; i++){
		buffer->addSample(sample);
		sample += 1.0;
	}
	ASSERT_EQUAL(3, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(29.0, buffer->getAverage());
}

void TestSlidingWindowBuffer::insertInvalidValueAsFirstValue(){
	float sample = -1.0;
	buffer->addSample(sample);
	ASSERT_EQUAL(0, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(true, isnanf(buffer->getAverage()) != 0);
}

void TestSlidingWindowBuffer::insertValidAndInvalidValue(){
	float validSample = 10.0;
	float invalidSample = -5.0;
	buffer->addSample(validSample);
	buffer->addSample(invalidSample);
	ASSERT_EQUAL(1, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(10.0, buffer->getAverage());
}

void TestSlidingWindowBuffer::testMinimumValueIsInfBeforeAnySampleIsAdded(){
	ASSERT_EQUAL(true, isinff(buffer->getMinimumValue()) == 1);
}

void TestSlidingWindowBuffer::insert5ValuesAndVerifyMinimum(){
	float sample = 15.0;
	for (int i = 0; i < 5; i++){
		buffer->addSample(sample);
		sample += 2;
	}
	ASSERT_EQUAL(3, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(false, isinff(buffer->getMinimumValue()) == 1);
	ASSERT_EQUAL(19.0, buffer->getMinimumValue());
	ASSERT_EQUAL(21.0, buffer->getAverage());
}

void TestSlidingWindowBuffer::testMaximumIsInfBeforeAnySampleIsAdded(){
	ASSERT_EQUAL(true, isinff(buffer->getMaximumValue()) == -1 || isinff(buffer->getMaximumValue()) == 1);
}

void TestSlidingWindowBuffer::insert10ValuesAndVerifyMaximum(){
	float sample = 30.0;
	for (int i = 0; i < 10; i++){
		buffer->addSample(sample);
		sample -= 1.0;
	}
	ASSERT_EQUAL(3, buffer->getNumberOfValidSamples());
	ASSERT_EQUAL(false, isinff(buffer->getMaximumValue()) == -1 || isinff(buffer->getMaximumValue()) == 1);
	ASSERT_EQUAL(23.0, buffer->getMaximumValue());
	ASSERT_EQUAL(22.0, buffer->getAverage());
}

void TestSlidingWindowBuffer::testNumberOfAttemptedSamples(){
	ASSERT_EQUAL(0, buffer->getNumberOfAttemptedSamples());
}

void TestSlidingWindowBuffer::testNumberOfAttemptedSamplesAfterInsertingValidAndInvalidSamples(){
	float sample = -4.0;
	for (int i = 0; i < 4; i++){
		buffer->addSample(sample);
		sample += 2.0;
	}
	ASSERT_EQUAL(3, buffer->getNumberOfAttemptedSamples());
	ASSERT_EQUAL(2, buffer->getNumberOfValidSamples());
}
