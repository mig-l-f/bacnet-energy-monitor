/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef SLIDINGWINDOWBUFFER_H_
#define SLIDINGWINDOWBUFFER_H_

#include "inttypes.h"
#include "math.h"
#include "limits.h"
#include "stddef.h"

class SlidingWindowBuffer{
public:
	SlidingWindowBuffer(unsigned int size, float minVal, float maxVal);
	~SlidingWindowBuffer();
	unsigned int getNumberOfValidSamples();
	unsigned int getWindowSize();
	void addSample(float & sampleValue);
	float getAverage();
	float getMinimumValue();
	float getMaximumValue();

private:
    void recalculate();
	unsigned int length;
	unsigned int numberOfAttemptedSamples;
	unsigned int numberOfValidSamples;
	unsigned int indexOfNextSample;
	float minimumThreshold;
	float maximumThreshold;
	float average;
	float* minimumValue;
	float* maximumValue;
	float* buffer;
};

#endif /* SLIDINGWINDOWBUFFER_H_ */
