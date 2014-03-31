/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef AVERAGINGNODE_H_
#define AVERAGINGNODE_H_

#include "BacnetNode.h"
#include "Averaging.h"
#include "AnalogValue.h"

class AveragingNode : public BacnetNode{
public:
	AveragingNode();
	~AveragingNode();
	static void handler_read_property(uint8_t * service_request, uint16_t service_len,
			BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_DATA * service_data);
	void makeNewMeasurement(float& measurement);
	void setPresentValue(float& value);
private:
	static AnalogValue* analog_value;
	static Averaging* average;

};

#endif
