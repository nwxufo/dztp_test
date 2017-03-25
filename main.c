/*
 *  main.c 	test DongZhongTong Android Client Software.
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-23 11:03
 */

#include <stdio.h>
#include <stdlib.h> //used for syscal : exit();
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/termios.h>

#include "serial_dev.c"

#define DEBUG
#define DEV_NAME "/dev/ttyS0"
#define BUF_LEN 204 // dzt_protocol's max length.
#define BUF_PARAM 199 //MAX protocol's parameter.


struct cmd_param {
	int len;
	unsigned char param[BUF_PARAM];
};

struct dzt_protocol{
	unsigned char head;
	unsigned char length;
	unsigned char addr; 
	unsigned char cmd;
	struct cmd_param param;
	unsigned char checkout;
	unsigned char end;
};

enum recv_cmd_type  {RECOVERY,VERSION_CHECK,OPERATION,CHECKOUT};
struct response_struct {
	enum recv_cmd_type type;
	int fd;		//tty devices file descriptor
	struct dzt_protocol dztp;
};

static struct dzt_protocol init_dzt_protocol( const char *msg )
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

static void dzt_proto_printer( struct dzt_protocol dztp ) 
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

static void msg_response_recovery(struct response_struct res_obj) {
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
static void msg_response_operation(struct response_struct res_obj)
{
	printf("reviced operation message !\n");
}
static void msg_response_checkout(struct response_struct res_obj)
{
	printf("reviced checkout message !\n");
}

void (*msg_response_tbl[])(struct response_struct) = {
	msg_response_recovery,
	msg_response_version,
	msg_response_operation,
	msg_response_checkout
};
#define msg_response(z) msg_response_tbl[z.type](z)

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
		res_obj.type = OPERATION;
	} else 
	if ( 0xA0 == ( res_obj.dztp.cmd & 0xA0 ) ) {
		res_obj.type = CHECKOUT;
	} else {
		printf( "cmd unrecognised !\n" );
		//protocol_crc(msg);
#ifdef DEBUG
		printf("cmd & 0xA0: %.04x\n", res_obj.dztp.cmd & 0xA0 );
#endif
		//print it!
		printf("%c ",msg[0]);
		printf("%.4x ", msg[1]);
		int len = (int)msg[1];
		int i = 0;
		while( i< len ) {
			printf("%.4x ",msg[i+2]);
			i++;
		}
		printf("%c\n", msg[len+2]);
		//dzt protocol printer.
		dzt_proto_printer(res_obj.dztp);
		exit(-1);
	}

	return res_obj;
}

static void  msg_processor(int fd, const char* msg)
{
	//struct dzt_protocol dztp;
	struct response_struct res_obj; 
	res_obj = init_response_struct( fd, msg);
	//dztp = msg_translator(res_obj,msg);
	dzt_proto_printer( res_obj.dztp );

	msg_response(res_obj);
}
void main(int argc, char *argv[])
{
	char message[BUF_LEN];
	int fd =  open_tty(DEV_NAME);

	while(1){
		int ret_val = read(fd, message, BUF_LEN);

		printf("message length: %d bytes\n", ret_val);

		msg_processor(fd,message);
	}
}	
