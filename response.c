/*
 *  response.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-25 16:14
 */

#include "response.h"
#include "protocol.h"
#include "dict_res_req.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG
extern struct dzt_protocol init_dzt_protocol( const unsigned char *msg )
{
	struct dzt_protocol dztp;
	/* encapsulation the msg to protocol struct. */
	dztp.head  = msg[0];
	dztp.length = msg[1];
	dztp.addr   = msg[2];
	dztp.cmd    = msg[3];
	dztp.param.len = (int)dztp.length - 3;
	int i = dztp.param.len ;
	int j = 0;
	while ( j < i ) {
		dztp.param.param[j] = msg[4+j];
		j++;
	}
	dztp.checkout = msg[4+i];//TODO: CRC
	dztp.end = msg[4+i+1];

	return dztp;
}

extern void dzt_proto_printer( struct dzt_protocol dztp ) 
{
	printf("%c ", dztp.head);
	printf("%.4x ", dztp.length);
	printf("%.4x ", dztp.addr);
	printf("%.4x ", dztp.cmd);

	int i = dztp.param.len;
	int j = 0 ;
	while ( j < i ) {
		printf("%.4x ", dztp.param.param[j]);
		j++;
	}

	printf("%.4x ", dztp.checkout);
	printf("%c", dztp.end);
	printf("\n");
}

/* response_pram_recovery */
static struct cmd_param get_response_param_recovery(struct dzt_protocol dztp)
{
	struct cmd_param response_param_recovery = {
		.len = 1,
		.param = {0x55},
	};
	
	return response_param_recovery;
}
/* response_param_version */
static struct cmd_param get_response_param_version(struct dzt_protocol dztp)
{
	struct cmd_param response_param_version = {
		.len = 14,
		.param = {0x02,0x2c,0x01,0x2c,0x01,0x2c,0x01,0x2c,0x01,0x2c,
			0x12,0x34,0x56,0x78},
	};
#ifdef DEBUG
	int i = 0;
	for(i;i<response_param_version.len;++i) {
		printf("param_version:%.2x",response_param_version.param[i]);
	}
	printf("\n");
#endif

	return response_param_version;
}
/* response_param_setting */
static struct cmd_param get_response_param_setting(struct dzt_protocol dztp)
{
	struct cmd_param response_param_setting = {
		.len = 1,
		.param = {0x55},
	};

	return response_param_setting;
}
/*TODO: response_param_checkout */
static struct cmd_param get_response_param_checkout(struct dzt_protocol dztp) 
{
	return get_req_res_param(dztp.cmd);
}
struct cmd_param (*get_response_param_tbl[]) (struct dzt_protocol) = {
	get_response_param_recovery,
	get_response_param_version,
	get_response_param_setting,
	get_response_param_checkout,
};
#define get_response_param(z) get_response_param_tbl[z.type](z.dztp)

static struct response_struct init_response_struct( int fd, const char* msg )
{
	struct response_struct res_obj;
	res_obj.fd = fd;
	res_obj.dztp = init_dzt_protocol(msg);
	if ( 0x05 == res_obj.dztp.cmd ) {
		res_obj.type = RECOVERY;
	} else 
	if ( 0x81 == res_obj.dztp.cmd ) {
		res_obj.type = VERSION_CHECK; 
	} else
	if ( 0x50 == (res_obj.dztp.cmd & 0x50) ) {
		res_obj.type = SETTING;
	} else 
	if ( 0xA0 == ( res_obj.dztp.cmd & 0xA0 ) ) {
		res_obj.type = CHECKOUT;
	} else {
		printf( "cmd unrecognised !\n" );
		exit(-1);
	}

	return res_obj;
}

void  msg_processor(int fd, const unsigned char* msg_recv)
{
	struct response_struct res_obj; 
	res_obj = init_response_struct(fd, msg_recv);
#ifdef DEBUG
	dzt_proto_printer( res_obj.dztp );
#endif
	/*get response param */
	res_obj.dztp.param = get_response_param(res_obj);
	/* modify dztp's len */
	res_obj.dztp.length = (unsigned char)( res_obj.dztp.param.len + 3 );
	/* transmit it. */
	int msg_len = res_obj.dztp.param.len +6;
	unsigned char msg_res[msg_len];
	dzt_protocol_to_msg(&(res_obj.dztp),msg_res);

	/* CRC */
	/*msg_res[msg_len-2] = get_crc(msg_res, msg_len);*/

	write(res_obj.fd,msg_res,msg_len);	
}
