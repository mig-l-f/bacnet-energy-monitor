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
AnalogValue** BacnetNode::analogValueList(new AnalogValue*[MAX_ANALOG_VALUES]);

BacnetNode::BacnetNode() {
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY, handler_read_property);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);

	//initilize list as null
	for(uint8_t i =0; i < MAX_ANALOG_VALUES; i++)
		analogValueList[i] = NULL;
	analogValueList[0] = new AnalogValue((uint32_t)100, "TestAnalogValue", "Test AV Object", UNITS_DEGREES_CELSIUS);
	//Add to Device Object List
	device->addObjectIdToObjectList(*(analogValueList[0]->getObjectIdentifier()));
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
AnalogValue* BacnetNode::getAnalogObjectFromList(uint8_t index){
	if (index < MAX_ANALOG_VALUES)
		return analogValueList[index];
	return NULL;
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
void BacnetNode::handler_read_property(uint8_t * service_request, uint16_t service_len,
		    BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_DATA * service_data){

	BACNET_READ_PROPERTY_DATA rpdata;
    int len = 0;
    int pdu_len = 0;
    int apdu_len = -1;
    int npdu_len = -1;
    BACNET_NPDU_DATA npdu_data;
    BACNET_ADDRESS my_address;

	/* configure default error code as an abort since it is common */
	rpdata.error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
	/* encode the NPDU portion of the packet */
	datalink_get_my_address(&my_address);
	npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
	npdu_len = npdu_encode_pdu(&Handler_Transmit_Buffer[0], src, &my_address, &npdu_data);

	if (service_data->segmented_message) {
		len = BACNET_STATUS_ABORT;
		dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
		return;
	}
	len = rp_decode_service_request(service_request, service_len, &rpdata);
	if (len <= 0){
		 /* bad decoding - skip to error/reject/abort handling */
		 dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
		 return;
	}
	/* Test for case of indefinite Device object instance */
	if ((rpdata.object_type == OBJECT_DEVICE) &&
		(rpdata.object_instance == BACNET_MAX_INSTANCE)) {
		rpdata.object_instance = device->getObjectIdentifier()->instance;
	}

	apdu_len = rp_ack_encode_apdu_init(&Handler_Transmit_Buffer[npdu_len],
			service_data->invoke_id, &rpdata);

	/* configure our storage */
	rpdata.application_data = &Handler_Transmit_Buffer[npdu_len + apdu_len];
	rpdata.application_data_len = sizeof(Handler_Transmit_Buffer) - (npdu_len + apdu_len);

	switch(rpdata.object_type){
		case OBJECT_DEVICE:
			len = device->Object_Read_Property(&rpdata);
			break;
		case OBJECT_ANALOG_VALUE:
			len = analogValueList[0]->Object_Read_Property(&rpdata);
			break;
		default:
			//FIXME: What happens when not implemented object specified
			break;
	}

	if (len >= 0){
		apdu_len += len;
		len = rp_ack_encode_apdu_object_property_end(&Handler_Transmit_Buffer[npdu_len + apdu_len]);
		apdu_len += len;
		if (apdu_len > service_data->max_resp) {
			/* too big for the sender - send an abort
			 * Setting of error code needed here as read property processing may
			 * have overriden the default set at start */
			rpdata.error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
			len = BACNET_STATUS_ABORT;
			dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
			return;
		}
	}else {
		dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
		return;
	}
	pdu_len = npdu_len + apdu_len;
	datalink_send_pdu(src, &npdu_data, &Handler_Transmit_Buffer[0], pdu_len);
	VERBOSE_STRING(PSTR(" --- End Request: Response Sent ---\n"));
#if defined(__AVR_ATmega328P__)
		uint16_t sSize = stack_size();
		uint16_t freeStack = freeRam();
		uint16_t hSize = heapSize();
		VERBOSE_STRING_INT(PSTR("StackSize: %d HeapSize: %d Free: %d\n"),sSize, hSize, freeStack);
#endif
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
