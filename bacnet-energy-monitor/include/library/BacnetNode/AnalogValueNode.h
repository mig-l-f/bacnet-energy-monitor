/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ANALOGVALUENODE_H_
#define ANALOGVALUENODE_H_

#include "BacnetNode.h"
#include "AnalogValue.h"

class AnalogValueNode : public BacnetNode{
public:
	AnalogValueNode();
	~AnalogValueNode();
	static void handler_read_property(uint8_t * service_request, uint16_t service_len,
				BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_DATA * service_data);
	void setPresentValue(float& value);
private:
	static AnalogValue* analog_value;
};

#endif
