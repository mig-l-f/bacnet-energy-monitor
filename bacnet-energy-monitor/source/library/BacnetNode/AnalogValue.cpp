/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "AnalogValue.h"

AnalogValue::AnalogValue(const uint32_t objectID, const char* objectName, const char* description, BACNET_ENGINEERING_UNITS units):
	AnalogObject(objectID, OBJECT_ANALOG_VALUE, objectName, description, units){};

AnalogValue::~AnalogValue(){}


void AnalogValue::setPresentValue(float value){
	Present_Value = value;
}

int AnalogValue::Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata){
	int apdu_len = 0;   /* return value */
	//int len = 0;        /* apdu len intermediate value */
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
		case PROP_PRESENT_VALUE:
			apdu_len =  encode_application_real(&apdu[0], Present_Value);
			break;
		case PROP_DESCRIPTION:
			BACNET_CHARACTER_STRING char_string;
			characterstring_init_ansi(&char_string, Description.value);
			apdu_len = encode_application_character_string(&apdu[0], &char_string);
			break;
		case PROP_STATUS_FLAGS:
			apdu_len = encode_application_bitstring(&apdu[0], &Status_Flags);
			break;
		case PROP_EVENT_STATE:
			apdu_len = encode_application_enumerated(&apdu[0], Event_State);
			break;
		case PROP_RELIABILITY:
			apdu_len = encode_application_enumerated(&apdu[0], Reliability);
			break;
		case PROP_OUT_OF_SERVICE:
			apdu_len = encode_application_boolean(&apdu[0], Out_Of_Service);
			break;
		case PROP_UNITS:
			apdu_len = encode_application_enumerated(&apdu[0], Units);
			break;
		default:
			rpdata->error_class = ERROR_CLASS_PROPERTY;
			rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
			apdu_len = BACNET_STATUS_ERROR;
			break;
	}
	return apdu_len;
}
