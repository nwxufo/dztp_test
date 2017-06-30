/*
 *  protocol.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-25 16:15
 */
#include "protocol.h"

static void set_crc(struct dzt_protocol * const dztpp)
{
	unsigned char crc = 0x00;
	crc += dztpp->cmd;
	crc += dztpp->length;
	crc += dztpp->addr;
	int i;
	for(i = 0; i < dztpp->param.len; ++i)
		crc += dztpp->param.param[i];
	dztpp->checkout = crc;
}

unsigned char get_crc(const unsigned char* msg, const int len)
{
	unsigned char ret_crc = 0x00;
	int i = 1;

	for ( i; i < len - 2; i++) {
		ret_crc += msg[i];
	}

	return ret_crc;
}

void dzt_protocol_to_msg(struct dzt_protocol * const dztpp, unsigned char msg[])
{
	 set_crc(dztpp);
	 msg[0] = dztpp->head;
	 msg[1] = dztpp->length;
	 msg[2] = dztpp->addr;
	 msg[3] = dztpp->cmd;
	 int i = 0;
	 int msg_index = 4;
	 for(i;i<dztpp->param.len;i++,msg_index++) 
		 msg[msg_index] = dztpp->param.param[i];
	 	
	 msg[msg_index] = dztpp->checkout;
	 msg[msg_index+1] = dztpp->end;
#ifdef DEBUG
	 msg_printer_raw(msg,msg_index+2);
#endif
}
