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


static void msg_response_recovery(struct response_struct res_obj)
{
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
	write(res_obj.fd, msg_res, 7);	
}
static void msg_response_version(struct response_struct res_obj) 
{
	printf("reviced version request  message !\n");
}
static void msg_response_setting(struct response_struct res_obj)
{
	printf("reviced setting message !\n");
}
static void msg_response_checkout(struct response_struct res_obj)
{
	printf("reviced checkout message !\n");
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
