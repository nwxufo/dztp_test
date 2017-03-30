/*
 *  main.c 	test DongZhongTong Android Client Software.
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-23 11:03
 */


#include "serial_cfg.h"
#include "protocol.h"
#include "response.h"

#include <stdio.h>
#include <stdlib.h> //used for syscal : exit();

#define DEBUG
#define DEV_NAME "/dev/ttyS0"
#define BUF_LEN 204 // dzt_protocol's max length.


static void msg_printer_raw(const char* msg, int len)
{
	int i = 0;
	while( i< len){
		printf("%.4x ",msg[i]);
		//printf("%.4x,%ld ",msg[i],sizeof(msg[i]));
		++i;
	}
	printf("\n");
}
static void  msg_processor(int fd, const char* msg)
{
	struct response_struct res_obj; 
	res_obj = init_response_struct(fd, msg);
#ifdef DEBUG
	dzt_proto_printer( res_obj.dztp );
#endif
	msg_response(res_obj);
}

static int get_msg(int fd, char *msg, int msg_buf)
{
	int len, msg_tail;
	msg_tail = len = 0; 

	char buf[BUF_LEN];
	char tail;
	while(msg_tail< msg_buf) {
		len = read(fd,buf,msg_buf);
		tail = buf[len-1];
		
		// append buf to msg.
		int j = 0;
		for(j;j<len;j++,msg_tail++)
			msg[msg_tail] = buf[j];
#ifdef DEBUG
		msg_printer_raw(msg,msg_tail);
#endif
		if ( (tail & 0x3e) == 0x3e )
			return msg_tail; // return msg length.
	}
	return -1;
}

void main(int argc, char *argv[])
{
	char message[BUF_LEN];
	int fd =  open_tty(DEV_NAME);
	
	while(1) {
	int ret_val = get_msg(fd,message,BUF_LEN); 
	printf("message length: %d bytes\n", ret_val);

	msg_processor(fd,message);
	}
}	
