/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
#include "AnalogValueNode.h"

AnalogValue* AnalogValueNode::analog_value = new AnalogValue(57, "ANALOG-1", "", UNITS_DEGREES_CELSIUS, 50.0, -10.0);

AnalogValueNode::AnalogValueNode(){
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY, handler_read_property);

	analog_value->setPresentValue(-1);
}

AnalogValueNode::~AnalogValueNode(){

}

void AnalogValueNode::handler_read_property(uint8_t * service_request, uint16_t service_len,
			BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_DATA * service_data){
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_NPDU_DATA npdu_data;
	BACNET_ADDRESS my_address;
	int16_t len = 0;
	int16_t pdu_len = 0;
	int16_t apdu_len = -1;
	int16_t npdu_len = -1;
	// configure default error code as an abort since it is common

	// encode the NPDU portion of the packet
	datalink_get_my_address(&my_address);
	npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
	npdu_len = (int16_t)npdu_encode_pdu(&Handler_Transmit_Buffer[0], src, &my_address, &npdu_data);

	if (service_data->segmented_message) {
		rpdata.error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
		len = BACNET_STATUS_ABORT;
		dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
		return;
	}

	len = (int16_t)rp_decode_service_request(service_request, service_len, &rpdata);
	// verify len
	if (len <= 0){
		 // bad decoding - skip to error/reject/abort handling
		 dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
		 return;
	}
	// Test for case of indefinite Device object instance

	apdu_len = (int16_t)rp_ack_encode_apdu_init(&Handler_Transmit_Buffer[npdu_len],
				service_data->invoke_id, &rpdata);
	// configure our storage
	rpdata.application_data = &Handler_Transmit_Buffer[npdu_len + apdu_len];
	rpdata.application_data_len = sizeof(Handler_Transmit_Buffer) - (npdu_len + apdu_len);
	len = 0; // re-initialize counter
	switch(rpdata.object_type){
		case OBJECT_DEVICE:
			len = device->Object_Read_Property(&rpdata);
			break;
		case OBJECT_ANALOG_VALUE:
			len = analog_value->Object_Read_Property(&rpdata);
			break;
		default:
			rpdata.error_class = ERROR_CLASS_OBJECT;
			rpdata.error_code = ERROR_CODE_UNKNOWN_OBJECT;
			len = BACNET_STATUS_ERROR;
			break;
	}
	if (len >= 0){
		apdu_len += len;
		len = (int16_t)rp_ack_encode_apdu_object_property_end(&Handler_Transmit_Buffer[npdu_len + apdu_len]);
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
	}else{
		dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
		return;
	}
	pdu_len = npdu_len + apdu_len;
	datalink_send_pdu(src, &npdu_data, &Handler_Transmit_Buffer[0], pdu_len);
}

void AnalogValueNode::setPresentValue(float& value){
	analog_value->setPresentValue(value);
}
