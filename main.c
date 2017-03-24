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

#include "serial_dev.h"

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
/*
struct msg_cmd {
	recv_cmd_type type;
	int fd;
};
*/

static int fd = 0; //tty devices file descriptor

static void dzt_proto_printer( struct dzt_protocol dztp ) ;
static struct dzt_protocol msg_translator( const char *msg )
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
	dztp.checkout = msg[4+i];//TODO: jiaoyan
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

static void response_recovery_msg() {
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
	write(fd, msg_res, 7);	
}
static void response_version_msg() 
{
	printf("reviced checkout version message !\n");
}

static void  msg_processor(const char* msg)
{
	struct dzt_protocol dztp;
	dztp = msg_translator(msg);
	dzt_proto_printer( dztp );
	if ( 0x05 == dztp.cmd & 0x05 ) { // device repositon
		response_recovery_msg();
	} else if ( 0x81 == dztp.cmd ) {
		response_version_msg();
	} else if ( 0xA0 == dztp.cmd & 0xA0 ) {
		response_checkout_msg();
	} else {
		printf("received cmd unrecognizable !\n");
	}
}

void main(int argc, char *argv[])
{
	char message[BUF_LEN];

	const char * dev = DEV_NAME; 
	fd = open_tty(dev);

	while(1){
		int ret_val = read(fd,message,BUF_LEN);

		printf("message length: %d bytes\n", ret_val);

		msg_processor(message);
	}
}	
