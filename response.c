/*
 *  response.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-25 16:14
 */

#include "response.h"
#include "stdlib.h"

extern struct dzt_protocol init_dzt_protocol(const char* msg);
extern void dzt_proto_printer( struct dzt_protocol dztp ) ;

extern struct dzt_protocol init_dzt_protocol( const char *msg )
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
static void msg_printer_format(const char* msg) 
{
	printf("%c ",msg[0]);
	printf("%.4x ", msg[1]);
	int len = (int)msg[1];
	int i = 0;
	while( i< len ) {
		printf("%.4x ",msg[i+2]);
		i++;
	}
	printf("%c\n", msg[len+2]);
}

/* response_pram_recovery */
static struct cmd_param response_param_recovery = {
	.len = 1,
	.param = {0x55},
};
/* response_param_version */
static struct cmd_param response_param_version = {
	.len = 14,
	.param = {0x02,0x2c,0x01,0x2c,0x01,0x2c,0x01,0x2c,0x01,0x02,
		0x12,0x34,0x56,0x78},
};
/* response_param_setting */
static struct cmd_param response_param_setting = {
	.len = 1,
	.param = {0x55},
};

/*TODO: response_param_checkout */
static struct cmd_param response_param_checkout = {
	.len = 10,
	.param = "UNFINISHED",
};

static void dzt_protocol_to_msg(const struct dzt_protocol dztp, unsigned char* msg)
{
	 msg[0] = dztp.head;
	 msg[1] = dztp.length;
	 msg[2] = dztp.addr;
	 msg[3] = dztp.cmd;
	 int i = 0;
	 int msg_index = 4;
	 for(i;i<dztp.param.len;i++) {
		 msg[msg_index] = dztp.param.param[i];
		 ++msg_index;++i;
	 }	
	 msg[msg_index] = dztp.checkout;
	 msg[msg_index+1] = dztp.end;

}
static void msg_response_recovery(struct response_struct res_obj)
{
	/* this code is ok,but old.
	unsigned int isSucceed = 1;
	unsigned char msg_res[7];
		msg_res[0] = 0x3c;
		msg_res[1] = 0x04;
		msg_res[2] = 0x00;
		msg_res[3] = 0x05;
		if (isSucceed) {
			msg_res[4] = 0x55;
			msg_res[5] = 0x5E;
		} else {
			msg_res[4] = 0xAA;
			msg_res[5] = 0xB3;
		}
		msg_res[6] = 0x3E;
	*/
	/* param */
	res_obj.dztp.param = response_param_recovery;
	/* TODO:CRC */
	res_obj.dztp.checkout = 0x5e;
	/* transmit it. */
	int msg_len = res_obj.dztp.param.len +6;
	unsigned char msg[msg_len];
	dzt_protocol_to_msg(res_obj.dztp,msg);

	printf("received recovery message !");
	msg_printer_format(msg);
	write(res_obj.fd,msg,msg_len);	
}
static void msg_response_version(struct response_struct res_obj) 
{
	printf("received version request  message !\n");
}
static void msg_response_setting(struct response_struct res_obj)
{
	printf("received setting message !\n");
}
static void msg_response_checkout(struct response_struct res_obj)
{
	printf("received checkout message !\n");
}
void (*msg_response_tbl[])(struct response_struct) = {
	msg_response_recovery,
	msg_response_version,
	msg_response_setting,
	msg_response_checkout
};
extern struct response_struct init_response_struct( int fd, const char* msg )
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
