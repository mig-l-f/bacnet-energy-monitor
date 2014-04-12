/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SlidingWindowBuffer.h"

SlidingWindowBuffer::SlidingWindowBuffer(unsigned int size, float minVal, float maxVal){
	length = size;
	buffer = new float[length];
	numberOfAttemptedSamples = 0;
	numberOfValidSamples = 0;
	indexOfNextSample = 0;
	minimumThreshold = minVal;
	maximumThreshold = maxVal;
	minimumValue = NULL;
	maximumValue = NULL;
	average = -1;
}

SlidingWindowBuffer::~SlidingWindowBuffer(){
	delete[] buffer;
}

unsigned int SlidingWindowBuffer::getNumberOfValidSamples(){
	return numberOfValidSamples;
}

unsigned int SlidingWindowBuffer::getWindowSize(){
	return length;
}

void SlidingWindowBuffer::addSample(float & sampleValue){

	/* Attempted samples for current window is the number of invalid and valid samples in the current window,
	 * cannot be higher than window samples*/
	if (numberOfAttemptedSamples < length)
		numberOfAttemptedSamples++;

	/* Verify validity of sample */
	bool isSampleValid = false;
	if (sampleValue >= minimumThreshold && sampleValue <= maximumThreshold)
		isSampleValid = true;

	if(!isSampleValid){
		return;
	}else if (isSampleValid && numberOfValidSamples < length){
		numberOfValidSamples++; // Only maximum number of samples is the window_samples number(length)
	}

	buffer[indexOfNextSample] = sampleValue;
	indexOfNextSample++;
	if (indexOfNextSample == length)
		indexOfNextSample = 0;

	recalculate();
}

float SlidingWindowBuffer::getAverage(){
	if (numberOfValidSamples < 1)
		return NAN;

	return average;
}

void SlidingWindowBuffer::recalculate(){
	float currentAverage = 0.0;

	unsigned int i = indexOfNextSample -1; // index that will count the values backwards
	for (unsigned int counter = 0; counter < numberOfValidSamples; counter++){
		/* Verify if current index needs adjustment do to arrive in the begginnig of buffer*/
		if (i == UINT_MAX) //unsigned int is circular so value -1 is UINT_MAX (the value before 0 is the maximum)
			i = length-1;

		currentAverage += buffer[i];

		if (minimumValue == NULL)
			minimumValue = &buffer[i];
		else if (buffer[i] < *minimumValue)
			minimumValue = &buffer[i];

		if (maximumValue == NULL)
			maximumValue = &buffer[i];
		else if (buffer[i] > *maximumValue)
			maximumValue = &buffer[i];

		i--;
	}

	average = currentAverage/(float)numberOfValidSamples;
}

float SlidingWindowBuffer::getMinimumValue(){
	if (minimumValue == NULL)
		return INFINITY;

	return (*minimumValue);
}

float SlidingWindowBuffer::getMaximumValue(){
	if (maximumValue == NULL)
		return -INFINITY;

	return (*maximumValue);
}

unsigned int SlidingWindowBuffer::getNumberOfAttemptedSamples(){
	return numberOfAttemptedSamples;
}
