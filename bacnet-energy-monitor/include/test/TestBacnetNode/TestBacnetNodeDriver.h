/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTBACNETNODEDRIVER_H_
#define TESTBACNETNODEDRIVER_H_

#include "bacdef.h"
#include "BacnetNode.h"

class TestBacnetNodeDriver{
public:
	TestBacnetNodeDriver(){};
	void setupAddress(BACNET_ADDRESS & src, BACNET_ADDRESS & dest);
	int makeReadPropertyRequestStub(BACNET_NPDU_DATA& npdu_data, BACNET_ADDRESS& dest,
			BACNET_ADDRESS& my_address, BACNET_READ_PROPERTY_DATA& data, BacnetNode* bacnetNode,
			uint8_t& invoke_id, bool useMyRpEncode);
	void handlerNpduStubAtServer(uint8_t * pdu, BACNET_NPDU_DATA& npdu_data, int& pdu_len);
	int myrp_encode_apdu(uint8_t * apdu, uint8_t invoke_id, BACNET_READ_PROPERTY_DATA * rpdata);
	int sendWhoIsNpdu(BACNET_NPDU_DATA& npdu_data, BACNET_ADDRESS & target_address,
			int32_t & low_limit, int32_t high_limit);
	void setupAddressForWhoIsRequest(BACNET_ADDRESS & addr);
private:
};

inline void TestBacnetNodeDriver::setupAddress(BACNET_ADDRESS & src, BACNET_ADDRESS & dest){
	int i = 0;
	dest.mac_len = 6;
	for (i = 0; i < dest.mac_len; i++) {
		dest.mac[i] = i;
	}
	/* DNET,DLEN,DADR */
	dest.net = 1;
	dest.len = 6;
	for (i = 0; i < dest.len; i++) {
		dest.adr[i] = i * 10;
	}
	src.mac_len = 1;
	for (i = 0; i < src.mac_len; i++) {
		src.mac[i] = 0x80;
	}
	/* SNET,SLEN,SADR */
	src.net = 2;
	src.len = 1;
	for (i = 0; i < src.len; i++) {
		src.adr[i] = 0x40;
	}
}
inline int TestBacnetNodeDriver::makeReadPropertyRequestStub(BACNET_NPDU_DATA& npdu_data, BACNET_ADDRESS& dest,
		BACNET_ADDRESS& my_address, BACNET_READ_PROPERTY_DATA& data, BacnetNode* bacnetNode,
		uint8_t& invoke_id, bool useMyRpEncode){
	npdu_encode_npdu_data(&npdu_data, true, MESSAGE_PRIORITY_NORMAL);
	int pdu_len = npdu_encode_pdu(&Handler_Transmit_Buffer[0], &dest,
			&my_address, &npdu_data);

	int len = 0;
	if (useMyRpEncode){
		len = myrp_encode_apdu(&Handler_Transmit_Buffer[pdu_len], invoke_id, &data);
	}else{
		len = rp_encode_apdu(&Handler_Transmit_Buffer[pdu_len], invoke_id, &data);
	}

	pdu_len += len;
	return pdu_len;
};
inline void TestBacnetNodeDriver::handlerNpduStubAtServer(uint8_t * pdu, BACNET_NPDU_DATA& npdu_data, int& pdu_len){
	BACNET_ADDRESS src = { 0 };
	BACNET_ADDRESS dest = { 0 };
	int apdu_offset = 0;
	apdu_offset = npdu_decode(&pdu[0], &dest, &src, &npdu_data);
	ASSERT(apdu_offset != 0);
	ASSERT(npdu_data.network_layer_message == 0);
	if ((apdu_offset > 0) && (apdu_offset <= pdu_len)) {
		apdu_handler(&src, &pdu[apdu_offset], (uint16_t)(((pdu_len - apdu_offset))));
	}
};
inline int TestBacnetNodeDriver::myrp_encode_apdu(uint8_t * apdu, uint8_t invoke_id, BACNET_READ_PROPERTY_DATA * rpdata){
	int len = 0;        /* length of each encoding */
	    int apdu_len = 0;   /* total length of the apdu, return value */

	    if (apdu) {
	        apdu[0] = PDU_TYPE_CONFIRMED_SERVICE_REQUEST;
	        apdu[1] = encode_max_segs_max_apdu(0, MAX_APDU);
	        apdu[2] = invoke_id;
	        apdu[3] = SERVICE_CONFIRMED_READ_PROPERTY;      /* service choice */
	        apdu_len = 4;
	        if (rpdata->object_type <= BACNET_MAX_OBJECT){
	        	len = encode_context_object_id(&apdu[apdu_len], 0, rpdata->object_type,
	        			rpdata->object_instance);
	        	apdu_len += len;
	        }
	        if (rpdata->object_property <= 4194303){
	        	len = encode_context_enumerated(&apdu[apdu_len], 1,
	        			rpdata->object_property);
	        	apdu_len += len;
	        }
	        /* optional array index */
	        if (rpdata->array_index != BACNET_ARRAY_ALL) {
	            len =
	                encode_context_unsigned(&apdu[apdu_len], 2,
	                rpdata->array_index);
	            apdu_len += len;
	        }
	    }

	    return apdu_len;
}
inline void TestBacnetNodeDriver::setupAddressForWhoIsRequest(BACNET_ADDRESS & addr){
	addr.mac_len = 1;
	int i;
	for (i = 0; i < addr.mac_len; i++) {
		addr.mac[i] = 0x80;
	}
	/* SNET,SLEN,SADR */
	addr.net = 2;
	addr.len = 1;
	for (i = 0; i < addr.len; i++) {
		addr.adr[i] = 0x40;
	}
}
inline int TestBacnetNodeDriver::sendWhoIsNpdu(BACNET_NPDU_DATA& npdu_data, BACNET_ADDRESS & target_address,
		int32_t & low_limit, int32_t high_limit){
	int len = 0;
	int pdu_len = 0;
	BACNET_ADDRESS my_address = { 0 };

	setupAddressForWhoIsRequest(my_address);

	/* encode the NPDU portion of the packet */
	npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);

	pdu_len = npdu_encode_pdu(&Handler_Transmit_Buffer[0], &target_address, &my_address, &npdu_data);
	/* encode the APDU portion of the packet */
	len = whois_encode_apdu(&Handler_Transmit_Buffer[pdu_len], low_limit, high_limit);
	pdu_len += len;
	//bytes_sent = datalink_send_pdu(target_address, &npdu_data, &Handler_Transmit_Buffer[0], pdu_len);
	return pdu_len;
}
#endif /* TESTBACNETNODEDRIVER_H_ */
