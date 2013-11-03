/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "DeviceObject.h"

void DeviceObject::InitializeVendorProperties() {
	Vendor_Identifier = VENDOR_IDENTIFIER;
#if defined(__AVR_ATmega328P__)
	char buffer[12];
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[0])));
	characterstring_init_ansi(&Vendor_Name, buffer);
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[1])));
	characterstring_init_ansi(&Model_Name, buffer);
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[2])));
	characterstring_init_ansi(&Firmware_Revision, buffer);
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[3])));
	characterstring_init_ansi(&Application_Software_Version, buffer);
#else
	characterstring_init_ansi(&Vendor_Name, VENDOR_NAME);
	characterstring_init_ansi(&Model_Name, MODEL_NAME);
	characterstring_init_ansi(&Firmware_Revision, FIRMWARE_REV);
	characterstring_init_ansi(&Application_Software_Version, SOFTWARE_VERSION);
#endif
}


void DeviceObject::InitializeObjectList() {
	Object_List[0].instance = getObjectIdentifier()->instance;
	Object_List[0].type = getObjectIdentifier()->type;
	++numberOfObjectsInList;
}

void DeviceObject::InitializeAPDUSettings() {
	APDU_Timeout = apdu_timeout();
	//Number_Of_APDU_Retries = apdu_retries();
}

DeviceObject::DeviceObject(const uint32_t objectID, const char* objectName,
		                   BACNET_DEVICE_STATUS deviceStatus) : BacnetObject(objectID, OBJECT_DEVICE, objectName){

	numberOfObjectsInList = 0;
	System_Status = deviceStatus;
	InitializeVendorProperties();
	InitializeObjectList();
	Max_APDU_Length_Accepted = MAX_APDU;
	Segmentation_Supported = SEGMENTATION_NONE;
	InitializeAPDUSettings();
	Database_Revision = 0;
}

DeviceObject::~DeviceObject() {}

uint16_t DeviceObject::getVendorIdentifier() const {
	return Vendor_Identifier;
}

BACNET_CHARACTER_STRING DeviceObject::getFirmwareRevision() const {
	return Firmware_Revision;
}

BACNET_CHARACTER_STRING DeviceObject::getSoftwareRevision() const {
	return Application_Software_Version;
}

BACNET_CHARACTER_STRING DeviceObject::getModelName() const {
	return Model_Name;
}

BACNET_CHARACTER_STRING DeviceObject::getVendorName() const {
	return Vendor_Name;
}

BACNET_DEVICE_STATUS DeviceObject::getDeviceStatus() const {
	return System_Status;
}

uint8_t DeviceObject::getProtocolVersion() const {
	return BACNET_PROTOCOL_VERSION;
}

uint8_t DeviceObject::getProtocolRevision() const {
	return BACNET_PROTOCOL_REVISION;
}

BACNET_BIT_STRING DeviceObject::getProtocolServicesSupported () {
	BACNET_BIT_STRING bit_string;
	bitstring_init(&bit_string);
	for (int i = 0; i < MAX_BACNET_SERVICES_SUPPORTED; i++) {
		bitstring_set_bit(&bit_string, (uint8_t) i, apdu_service_supported((BACNET_SERVICES_SUPPORTED) i));
	}
	return bit_string;
}

BACNET_BIT_STRING* DeviceObject::getProtocolObjectTypesSupported() {
	bitstring_init(&Protocol_Object_Types_Supported);
	for (int i = 0; i < MAX_ASHRAE_OBJECT_TYPE; i++) {
		bitstring_set_bit(&Protocol_Object_Types_Supported, (uint8_t) i, false);
	}
	for (int i = 0; i < numberOfObjectsInList; i++) {
		bitstring_set_bit(&Protocol_Object_Types_Supported, Object_List[i].type, true);
	}
	return &Protocol_Object_Types_Supported;
}

uint16_t DeviceObject::getApduTimeout() const {
	return APDU_Timeout;
}

BACNET_SEGMENTATION DeviceObject::getSegmentationSupported() const {
	return Segmentation_Supported;
}

uint32_t DeviceObject::getMaxApduLengthAccepted() const {
	return Max_APDU_Length_Accepted;
}

BACNET_OBJECT_ID* DeviceObject::getObjectList() {
	return Object_List;
}

uint32_t DeviceObject::getDatabaseRevision() const {
	return Database_Revision;
}

uint8_t DeviceObject::getNumberOfApduRetries() const {
	return 3;
}

unsigned DeviceObject::getCount() const {
	return MAX_BACNET_OBJECTS_PER_DEVICE;
}

bool DeviceObject::getValid_Object_Instance_Number(uint32_t object_id) {
	return ((getObjectIdentifier()->instance == object_id) || (object_id == BACNET_MAX_INSTANCE));
}


int DeviceObject::Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata) {

		int apdu_len = 0;   /* return value */
	    int len = 0;        /* apdu len intermediate value */
	    uint8_t *apdu = NULL;
	    BACNET_BIT_STRING bit_string;

	    if ((rpdata == NULL) || (rpdata->application_data == NULL) || (rpdata->application_data_len == 0)) {
	        return 0;
	    }
	    apdu = rpdata->application_data;
	    switch (rpdata->object_property) {
	    	case PROP_OBJECT_IDENTIFIER:
	    		apdu_len = encode_application_object_id(&apdu[0], getObjectIdentifier()->type, getObjectIdentifier()->instance);
	    		break;
	    	case PROP_OBJECT_NAME:
	    		apdu_len = encode_application_character_string(&apdu[0], getObjectName());
	    		break;
	    	case PROP_OBJECT_TYPE:
	    		apdu_len = encode_application_enumerated(&apdu[0], getObjectIdentifier()->type);
	    		break;
	    	case PROP_SYSTEM_STATUS:
	    		apdu_len = encode_application_enumerated(&apdu[0], System_Status);
	    		break;
	    	case PROP_VENDOR_NAME:
	    		apdu_len = encode_application_character_string(&apdu[0], &Vendor_Name);
	    		break;
	    	case PROP_VENDOR_IDENTIFIER:
	    		apdu_len = encode_application_unsigned(&apdu[0], getVendorIdentifier());
	    		break;
	    	case PROP_MODEL_NAME:
	    		apdu_len = encode_application_character_string(&apdu[0], &Model_Name);
	    		break;
	    	case PROP_FIRMWARE_REVISION:
	    		apdu_len = encode_application_character_string(&apdu[0], &Firmware_Revision);
	    		break;
	    	case PROP_APPLICATION_SOFTWARE_VERSION:
	    		apdu_len = encode_application_character_string(&apdu[0], &Application_Software_Version);
	    		break;
	    	case PROP_PROTOCOL_VERSION:
	    		apdu_len = encode_application_unsigned(&apdu[0], getProtocolVersion());
	    		break;
	    	case PROP_PROTOCOL_REVISION:
	    		apdu_len = encode_application_unsigned(&apdu[0], getProtocolRevision());
	    		break;
	    	case PROP_PROTOCOL_SERVICES_SUPPORTED:
	    		bit_string = getProtocolServicesSupported();
	    		apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
	    		break;
	    	case PROP_PROTOCOL_OBJECT_TYPES_SUPPORTED:
	    		apdu_len = encode_application_bitstring(&apdu[0], getProtocolObjectTypesSupported());
	    		break;
	    	case PROP_OBJECT_LIST:
	    		if (rpdata->array_index == 0) {// Return Count of Objects
	    			apdu_len = encode_application_unsigned(&apdu[0], MAX_BACNET_OBJECTS_PER_DEVICE);

	    		}else if (rpdata->array_index == BACNET_ARRAY_ALL){ // Return ID of all objects
	    			for (int i = 0; i < MAX_BACNET_OBJECTS_PER_DEVICE; i++) {

	    				len = encode_application_object_id(&apdu[apdu_len],
	    							Object_List[i].type, Object_List[i].instance);
	    				apdu_len += len;
	    				/* Next chunk does not fit in one apdu (>MAX_APDU)
	    				 * Segmentation not supported*/
	    				if ((i != MAX_BACNET_OBJECTS_PER_DEVICE) && (apdu_len + len >= MAX_APDU)){
	    					rpdata->error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
	    					apdu_len = BACNET_STATUS_ABORT;
	    					break;
	    				}
	    			}
	    		}else{ // Return Object ID of element pointed by array index
	    			if (rpdata->array_index <= MAX_BACNET_OBJECTS_PER_DEVICE) {

	    				VERBOSE_STRING_INT(PSTR("OBJ TYPE= %d, OBJ INST= %d\n"),
	    						getTypeFromObjectList(0), getInstanceFromObjectList(0));

	    				apdu_len = encode_application_object_id(&apdu[0],
	    						Object_List[rpdata->array_index-1].type,
	    						Object_List[rpdata->array_index-1].instance);
	    			} else {
	    				rpdata->error_class = ERROR_CLASS_PROPERTY;
	    				rpdata->error_code = ERROR_CODE_INVALID_ARRAY_INDEX;
	    				apdu_len = BACNET_STATUS_ERROR;
	    			}
	    		}
	    		break;
	    	case PROP_MAX_APDU_LENGTH_ACCEPTED:
	    		apdu_len = encode_application_unsigned(&apdu[0], getMaxApduLengthAccepted());
	    		break;
	    	case PROP_SEGMENTATION_SUPPORTED:
	    		apdu_len = encode_application_enumerated(&apdu[0], getSegmentationSupported());
	    		break;
	    	case PROP_APDU_TIMEOUT:
	    		apdu_len = encode_application_unsigned(&apdu[0], getApduTimeout());
	    		break;
	    	case PROP_NUMBER_OF_APDU_RETRIES:
	    		apdu_len = encode_application_unsigned(&apdu[0], getNumberOfApduRetries());
	    		break;
	    	case PROP_DEVICE_ADDRESS_BINDING:
	    		//apdu_len = address_list_encode(&apdu[0], MAX_APDU);
	    		break;
	    	case PROP_DATABASE_REVISION:
	    		apdu_len = encode_application_unsigned(&apdu[0], getDatabaseRevision());
	    		break;
	    	default:
	    		rpdata->error_class = ERROR_CLASS_PROPERTY;
	    		rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
	    		apdu_len = BACNET_STATUS_ERROR;
	    		break;
	    }

	    return apdu_len;
}

uint16_t DeviceObject::getTypeFromObjectList(uint8_t index) const{
	return Object_List[index].type;
}
uint32_t DeviceObject::getInstanceFromObjectList(uint8_t index) const{
	return Object_List[index].instance;
}

bool DeviceObject::addObjectIdToObjectList(BACNET_OBJECT_ID & objectId){
	if (numberOfObjectsInList >= MAX_BACNET_OBJECTS_PER_DEVICE){
		return false;
	}
	Object_List[numberOfObjectsInList].instance = objectId.instance;
	Object_List[numberOfObjectsInList].type = objectId.type;
	++numberOfObjectsInList;
	return true;
}
