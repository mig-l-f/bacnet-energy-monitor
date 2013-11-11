/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "BacnetNode.h"
#include <stdio.h> //FIXME: Added for debug
DeviceObject* BacnetNode::device(new DeviceObject( (uint32_t)55, "MiguelDevice", STATUS_OPERATIONAL));

BacnetNode::BacnetNode() {
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
}

#ifdef LOGGING
BacnetNode::BacnetNode(std::string & logLevel){
	FILELog::ReportingLevel() = FILELog::FromString(logLevel);
	FILE_LOG(logDEBUG) << "Created BacnetObject";
}
#endif


DeviceObject* BacnetNode::getDeviceObject() {
	return device;
}

void BacnetNode::dealWithErrorCodes(int & len, int& npdu_len,
									BACNET_CONFIRMED_SERVICE_DATA * service_data,
									BACNET_READ_PROPERTY_DATA& rpdata,
									BACNET_ADDRESS * src,
									BACNET_NPDU_DATA & npdu_data){
	int apdu_len = 0;
	switch(len){
		case BACNET_STATUS_REJECT:
			apdu_len = reject_encode_apdu(&Handler_Transmit_Buffer[npdu_len],
					service_data->invoke_id, reject_convert_error_code(rpdata.error_code));
			VERBOSE_STRING(PSTR("Sending reject status.\n"));
			break;
		case BACNET_STATUS_ABORT: //Not Covered by Unit Tests
			apdu_len = abort_encode_apdu(&Handler_Transmit_Buffer[npdu_len],
			        service_data->invoke_id, abort_convert_error_code(rpdata.error_code), true);

			VERBOSE_STRING(PSTR("Sending abort status.\n"));
			break;
		case BACNET_STATUS_ERROR: //Not Covered by Unit Tests
			apdu_len = bacerror_encode_apdu(&Handler_Transmit_Buffer[npdu_len],
			        service_data->invoke_id, SERVICE_CONFIRMED_READ_PROPERTY,
			        rpdata.error_class, rpdata.error_code);
			VERBOSE_STRING(PSTR("Sending error status.\n"));
			break;
	}
	int pdu_len = npdu_len + apdu_len;
	datalink_send_pdu(src, &npdu_data, &Handler_Transmit_Buffer[0], pdu_len);
	VERBOSE_STRING(PSTR("--- End Request: Response Sent ---\n"));

}

void BacnetNode::handler_who_is(uint8_t * service_request, uint16_t service_len,
		BACNET_ADDRESS * src){

    int len = 0;
    int32_t low_limit = 10;
    int32_t high_limit = 0;

    len = whois_decode_service_request(service_request, service_len, &low_limit, &high_limit);

    VERBOSE_STRING_INT(PSTR("Who-Is ObjectID %d, low limit = %d, high limit=%d\n"),
    		device->getObjectIdentifier()->instance, low_limit, high_limit);

    if (len == 0){
    	// respond When no limits specified
    	sendIamUnicast(&Handler_Transmit_Buffer[0], src);
    }else if (len != -1) {
    	// Only send when device Id is within limits .OR. Bacnet Wildcard where every devices respond
    	if ((device->getObjectIdentifier()->instance >= (uint32_t) low_limit) &&
    		(device->getObjectIdentifier()->instance <= (uint32_t) high_limit)){

    		sendIamUnicast(&Handler_Transmit_Buffer[0], src);
    	}else if ((BACNET_MAX_INSTANCE >= (uint32_t) low_limit) &&
    			 (BACNET_MAX_INSTANCE >= (uint32_t) high_limit)){

    		sendIamUnicast(&Handler_Transmit_Buffer[0], src);
    	}
    }

}
void BacnetNode::sendIamUnicast(uint8_t * buffer, BACNET_ADDRESS * src){

    BACNET_ADDRESS dest;
    int pdu_len = 0;
    BACNET_NPDU_DATA npdu_data;

    /* encode the data */
    int npdu_len = 0;
    int apdu_len = 0;
    BACNET_ADDRESS my_address;
    /* The destination will be the same as the src, so copy it over. */
    memcpy(&dest, src, sizeof(BACNET_ADDRESS));
    /* dest->net = 0; - no, must direct back to src->net to meet BTL tests */

    datalink_get_my_address(&my_address);
    /* encode the NPDU portion of the packet */
    npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
    npdu_len = npdu_encode_pdu(&buffer[0], &dest, &my_address, &npdu_data);
    /* encode the APDU portion of the packet */
    apdu_len = iam_encode_apdu(&buffer[npdu_len],
    							device->getObjectIdentifier()->instance,
    							MAX_APDU,
    							SEGMENTATION_NONE,
    							VENDOR_IDENTIFIER);
//    apdu_len = iam_encode_apdu(&buffer[npdu_len], //FIXME: Had to remove, would only reply when -1 was set
//    							device->getObjectIdentifier()->instance,
//    							device->getMaxApduLengthAccepted(),
//    							device->getSegmentationSupported(),
//    							device->getVendorIdentifier());
    /* send data */
    pdu_len = npdu_len + apdu_len;
    datalink_send_pdu(&dest, &npdu_data, &buffer[0], pdu_len);
    VERBOSE_STRING(PSTR(" --- End Request: Response Sent ---\n"));
#if defined(__AVR_ATmega328P__)
		uint16_t sSize = stack_size();
		uint16_t freeStack = freeRam();
		uint16_t hSize = heapSize();
		VERBOSE_STRING_INT(PSTR("StackSize: %d HeapSize: %d Free: %d\n"),sSize, hSize, freeStack);
#endif
}
