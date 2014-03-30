/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef AVERAGING_H_
#define AVERAGING_H_

#include "BacnetObject.h"
#include "SlidingWindowBuffer.h"
extern "C"{
	#include "bacenum.h"
	#include "bacdcode.h"
	#include "bacdevobjpropref.h"
	#include "bacapp.h"
}

class Averaging : public BacnetObject {
private:
	SlidingWindowBuffer* buffer;
	unsigned int Window_Samples; /* 12.5.15 number of samples to be taken during the period of time specified by the 'Window_Interval' property */
	unsigned int Window_Interval; /* 12.5.14 period of time in seconds over which the minimum, maximum and average values are calculated */
	BACNET_DEVICE_OBJECT_PROPERTY_REFERENCE object_property_reference;

public:
	Averaging(const uint32_t objectID, const char* objectName, unsigned int Window_Interval, unsigned int Window_Samples,
				  BacnetObject* referenced_object, BACNET_PROPERTY_ID referenced_property);
	Averaging(const uint32_t objectID, const char* objectName, unsigned int Window_Interval, unsigned int Window_Samples,
			  float validSampleMinimumThreshold, float validSampleMaximumThreshold,
			  BacnetObject* referenced_object, BACNET_PROPERTY_ID referenced_property);
	~Averaging();
	unsigned getCount() const;
	bool getValid_Object_Instance_Number(uint32_t object_id);
	int Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata);
	void addNewSample(float& new_sample);
};

inline unsigned Averaging::getCount() const{
	return 1;
}

inline bool Averaging::getValid_Object_Instance_Number(uint32_t object_id){
	return Object_Identifier.instance == object_id;
}

#endif /* AVERAGING_H_ */
