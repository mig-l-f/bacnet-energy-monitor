/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "Averaging.h"

Averaging::Averaging(const uint32_t objectID, const char* objectName, unsigned int windowInterval, unsigned int windowSamples,
					BacnetObject* referenced_object, BACNET_PROPERTY_ID referenced_property):BacnetObject(objectID, OBJECT_AVERAGING, objectName){
	Window_Samples = windowSamples;
	Window_Interval = windowInterval;
	object_property_reference.objectIdentifier.instance = referenced_object->getObjectIdentifier()->instance;
	object_property_reference.objectIdentifier.type = referenced_object->getObjectIdentifier()->type;
	object_property_reference.propertyIdentifier = referenced_property;

	float validSampleMinimumThreshold = 100.0;
	float validSampleMaximumThreshold = 0.0;
	BACNET_READ_PROPERTY_DATA read_property_data;
	read_property_data.object_property = PROP_LOW_LIMIT;
	read_property_data.array_index = BACNET_ARRAY_ALL;
	uint8_t apdu[15] = { 0 };
	read_property_data.application_data = &apdu[0];
	int apdu_len = referenced_object->Object_Read_Property(&read_property_data);
	if (apdu_len != BACNET_STATUS_ERROR){
		BACNET_APPLICATION_DATA_VALUE application_data_value;
		bacapp_decode_application_data(read_property_data.application_data, MAX_APDU, &application_data_value);
		validSampleMinimumThreshold = application_data_value.type.Real;
	}

	read_property_data.object_property = PROP_HIGH_LIMIT;
	read_property_data.array_index = BACNET_ARRAY_ALL;
	read_property_data.application_data = &apdu[0];
	apdu_len = referenced_object->Object_Read_Property(&read_property_data);
	if (apdu_len != BACNET_STATUS_ERROR){
		BACNET_APPLICATION_DATA_VALUE application_data_value;
		bacapp_decode_application_data(read_property_data.application_data, MAX_APDU, &application_data_value);
		validSampleMaximumThreshold = application_data_value.type.Real;
	}
	buffer = new SlidingWindowBuffer(Window_Samples, validSampleMinimumThreshold, validSampleMaximumThreshold);
}
Averaging::Averaging(const uint32_t objectID, const char* objectName, unsigned int windowInterval, unsigned int windowSamples,
					float validSampleMinimumThreshold, float validSampleMaximumThreshold, BacnetObject* referenced_object,
					BACNET_PROPERTY_ID referenced_property):BacnetObject(objectID, OBJECT_AVERAGING, objectName){
	Window_Samples = windowSamples;
	Window_Interval = windowInterval;
	buffer = new SlidingWindowBuffer(Window_Samples, validSampleMinimumThreshold, validSampleMaximumThreshold);
	object_property_reference.objectIdentifier.instance = referenced_object->getObjectIdentifier()->instance;
	object_property_reference.objectIdentifier.type = referenced_object->getObjectIdentifier()->type;
	object_property_reference.propertyIdentifier = referenced_property;
}

Averaging::~Averaging(){
	delete buffer;
}

int Averaging::Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata){
	int apdu_len = 0;   /* return value */
	uint8_t *apdu = NULL;

	if ((rpdata == NULL) || (rpdata->application_data == NULL) || (rpdata->application_data_len == 0)) {
		return 0;
	}
	if (rpdata->array_index != BACNET_ARRAY_ALL){ //Object does not have array properties
		rpdata->error_class = ERROR_CLASS_PROPERTY;
		rpdata->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
		return BACNET_STATUS_ERROR;
	}
	apdu = rpdata->application_data;
	switch (rpdata->object_property) {
		case PROP_OBJECT_NAME:
			apdu_len = encode_application_character_string(&apdu[0], &Object_Name);
			break;
		case PROP_OBJECT_IDENTIFIER:
			apdu_len = encode_application_object_id(&apdu[0], Object_Identifier.type, Object_Identifier.instance);
			break;
		case PROP_OBJECT_TYPE:
			apdu_len = encode_application_enumerated(&apdu[0], Object_Identifier.type);
			break;
		case PROP_MINIMUM_VALUE:
			apdu_len = encode_application_real(&apdu[0], buffer->getMinimumValue());
			break;
		case PROP_MAXIMUM_VALUE:
			apdu_len = encode_application_real(&apdu[0], buffer->getMaximumValue());
			break;
		case PROP_AVERAGE_VALUE:
			apdu_len = encode_application_real(&apdu[0], buffer->getAverage());
			break;
		case PROP_ATTEMPTED_SAMPLES:
			apdu_len = encode_application_unsigned(&apdu[0], buffer->getNumberOfAttemptedSamples());
			break;
		case PROP_VALID_SAMPLES:
			apdu_len = encode_application_unsigned(&apdu[0], buffer->getNumberOfValidSamples());
			break;
		case PROP_WINDOW_INTERVAL:
			apdu_len = encode_application_unsigned(&apdu[0], Window_Interval);
			break;
		case PROP_WINDOW_SAMPLES:
			apdu_len = encode_application_unsigned(&apdu[0], Window_Samples);
			break;
		case PROP_OBJECT_PROPERTY_REFERENCE:
			apdu_len = bacapp_encode_device_obj_property_ref(&apdu[0], &object_property_reference);
			break;
		default:
    		rpdata->error_class = ERROR_CLASS_PROPERTY;
    		rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
    		apdu_len = BACNET_STATUS_ERROR;
			break;
	}
	return apdu_len;
}

void Averaging::addNewSample(float& new_sample){
	buffer->addSample(new_sample);
}
