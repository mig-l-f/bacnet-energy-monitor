/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTBACNETNODESTUB_H_
#define TESTBACNETNODESTUB_H_

class TestBacnetNodeStub{
public:
	TestBacnetNodeStub(){};
	int handlerNpduStubAtClient(uint8_t * pdu, BACNET_NPDU_DATA& npdu_data,
			int& pdu_len, BACNET_READ_PROPERTY_DATA * data);
	int handlerNpduIamAtClient(uint8_t* pdu, BACNET_NPDU_DATA& npdu_data, int& pdu_len);
	void setupAddressForWhoIsRequest(BACNET_ADDRESS & addr);
private:
};

inline int TestBacnetNodeStub::handlerNpduStubAtClient(uint8_t * pdu, BACNET_NPDU_DATA& npdu_data,
		int& pdu_len, BACNET_READ_PROPERTY_DATA * data){
	BACNET_ADDRESS src = { 0 };
	BACNET_ADDRESS dest = { 0 };
	int apdu_offset = 0;
	apdu_offset = npdu_decode(&pdu[0], &dest, &src, &npdu_data);
	ASSERT(apdu_offset != 0);
	ASSERT(npdu_data.network_layer_message == 0);
	int len = 0;
	if(pdu[apdu_offset] == PDU_TYPE_CONFIRMED_SERVICE_REQUEST)
		return -1;

	else if(pdu[apdu_offset] == PDU_TYPE_COMPLEX_ACK){
		apdu_offset += 3;
		len = rp_ack_decode_service_request(&pdu[apdu_offset], (uint16_t)(((pdu_len - apdu_offset))), data);
	}
	else if(pdu[apdu_offset] == PDU_TYPE_ERROR){
		BACNET_CONFIRMED_SERVICE service = SERVICE_CONFIRMED_READ_PROPERTY;
		len = bacerror_decode_service_request(&pdu[apdu_offset + 1], pdu_len - 1, 0,
				&service, &data->error_class, &data->error_code);
	}
	return len;
};
inline int TestBacnetNodeStub::handlerNpduIamAtClient(uint8_t* pdu, BACNET_NPDU_DATA& npdu_data, int& pdu_len){
	BACNET_ADDRESS src = { 0 };
    int apdu_offset = 0;
    BACNET_ADDRESS dest = { 0 };
//    BACNET_NPDU_DATA npdu_data = { 0 };

	setupAddressForWhoIsRequest(src);

	if (pdu[0] == BACNET_PROTOCOL_VERSION) {
		apdu_offset = npdu_decode(&pdu[0], &dest, &src, &npdu_data);
		return apdu_offset;
	}else{
		return -1;
	}
};
inline void TestBacnetNodeStub::setupAddressForWhoIsRequest(BACNET_ADDRESS & addr){
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
#endif /* TESTBACNETNODESTUB_H_ */
