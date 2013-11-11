#include "BacnetNode2Thermos.h"

AnalogObject** BacnetNode2Thermos::analogValueList(new AnalogObject*[MAX_ANALOG_VALUES]);

BacnetNode2Thermos::BacnetNode2Thermos(){
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY, handler_read_property);
	//initilize list as null
	for(uint8_t i =0; i < MAX_ANALOG_VALUES; i++)
		analogValueList[i] = NULL;
	analogValueList[0] = new AnalogValue((uint32_t)1, "Analog Value 1", "Test AV Object 1", UNITS_DEGREES_CELSIUS);
	analogValueList[1] = new AnalogValue((uint32_t)2, "Analog Value 2", "Test AV Object 2", UNITS_DEGREES_CELSIUS);
	//Add to Device Object List
	device->addObjectIdToObjectList(*(analogValueList[0]->getObjectIdentifier()));
	device->addObjectIdToObjectList(*(analogValueList[1]->getObjectIdentifier()));
}

BacnetNode2Thermos::~BacnetNode2Thermos(){}

AnalogObject* BacnetNode2Thermos::getAnalogObjectFromList(uint8_t index){
	if (index < MAX_ANALOG_VALUES)
		return analogValueList[index];
	return NULL;
}

void BacnetNode2Thermos::handler_read_property(uint8_t * service_request, uint16_t service_len,
		    BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_DATA * service_data){

	BACNET_READ_PROPERTY_DATA rpdata;
    int len = 0;
    int pdu_len = 0;
    int apdu_len = -1;
    int npdu_len = -1;
    BACNET_NPDU_DATA npdu_data;
    BACNET_ADDRESS my_address;

	// configure default error code as an abort since it is common
	rpdata.error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
	// encode the NPDU portion of the packet
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
		 // bad decoding - skip to error/reject/abort handling
		 dealWithErrorCodes(len, npdu_len, service_data, rpdata, src, npdu_data);
		 return;
	}
	// Test for case of indefinite Device object instance
	if ((rpdata.object_type == OBJECT_DEVICE) &&
		(rpdata.object_instance == BACNET_MAX_INSTANCE)) {
		rpdata.object_instance = device->getObjectIdentifier()->instance;
	}

	apdu_len = rp_ack_encode_apdu_init(&Handler_Transmit_Buffer[npdu_len],
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
		{
			bool found = false;
			for(uint8_t i = 0; i < MAX_ANALOG_VALUES; i++){
				if(analogValueList[i]->isObject(rpdata.object_instance, OBJECT_ANALOG_VALUE)){
					len = analogValueList[rpdata.object_instance-1]->Object_Read_Property(&rpdata);
					found = true;
					break;
				}
			}
			if(!found){
				rpdata.error_class = ERROR_CLASS_OBJECT;
				rpdata.error_code = ERROR_CODE_UNKNOWN_OBJECT;
				len = BACNET_STATUS_ERROR;
			}
			break;
		}
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
