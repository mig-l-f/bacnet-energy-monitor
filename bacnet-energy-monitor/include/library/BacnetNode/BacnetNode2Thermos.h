/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef BACNETNODE2THERMOS_H_
#define BACNETNODE2THERMOS_H_

#include "BacnetNode.h"
#include "AnalogObject.h"
#include "AnalogValue.h"

class BacnetNode2Thermos: public BacnetNode {
public:
	BacnetNode2Thermos();
	~BacnetNode2Thermos();
	AnalogObject* getAnalogObjectFromList(uint8_t index);
	static void handler_read_property(uint8_t * service_request, uint16_t service_len,
						    BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_DATA * service_data);
private:
	static AnalogObject** analogValueList;
};

#endif
