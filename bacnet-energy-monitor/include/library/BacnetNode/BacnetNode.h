/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef BACNETNODE_H_
#define BACNETNODE_H_

//#include <cstring>
#include <string.h>
#include "apdu.h"
#include "npdu.h"
#include "txbuf.h"
#include "reject.h"
#include "bacerror.h"
#include "abort.h"
#include "whois.h"
#include "iam.h"
#include "datalink.h"
#include "DeviceObject.h"
#include "BacnetObject.h"
#include "AnalogValue.h"
#include "Constants.h"


#ifdef LOGGING
	#include "log.h"
#endif



enum BACNET_NODE_STATUS {NODE_OK, NODE_DOWN};

class BacnetNode {

public:
	BacnetNode();
#ifdef LOGGING
	BacnetNode(std::string & logLevel);
#endif
	~BacnetNode() {}
	DeviceObject* getDeviceObject();
	AnalogValue* getAnalogObjectFromList(uint8_t index);
	static void handler_read_property(uint8_t * service_request, uint16_t service_len,
					    BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_DATA * service_data);
	static void dealWithErrorCodes(int & len, int& npdu_len,
								   BACNET_CONFIRMED_SERVICE_DATA * service_data,
								   BACNET_READ_PROPERTY_DATA& rpdata,
								   BACNET_ADDRESS * src,
								   BACNET_NPDU_DATA & npdu_data);
	static void handler_who_is(uint8_t * service_request, uint16_t service_len,
		    			BACNET_ADDRESS * src);
	static void sendIamUnicast(uint8_t * buffer, BACNET_ADDRESS * src);
private:
	static DeviceObject* device;
	static AnalogValue** analogValueList;
};

#endif /* BACNETNODE_H_ */
