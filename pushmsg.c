/*
 *  pushmsg.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-04-17 09:03
 */

#include "pushmsg.h"
#include "protocol.h"

static const unsigned char pushcmd[4] = {0xf1,0xf2,0xf3,0xf4};
static int index = 0;

static struct dzt_protocol get_push_proto(const unsigned char pushcmd)
{
	struct dzt_protocol dztp;
	struct cmd_param push_cmd_param = { 
	.len = 0 ,
	.param[0] = 0,
	};
	dztp.head  = 0x3c;
	dztp.length = 0x03;
	dztp.addr   = 0x00;
	dztp.cmd    = pushcmd;	
	//dztp.checkout = set_crc(&dztp); automatic set crc.
	dztp.end = 0x3e;
	return dztp;
}

extern size_t autogen_push_msg(unsigned char msg[])
{
	const unsigned char acmd = pushcmd[index];
	index++;
	index = index%4;
	struct dzt_protocol dztp = get_push_proto(acmd);
	return dzt_protocol_to_msg(&dztp, msg );
}
