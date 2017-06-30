/*
 *  main.c 	test DongZhongTong Android Client Software.
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-23 11:03
 */


#include "serial_cfg.h"
#include "protocol.h"
#include "pushmsg.h"

#include <stdio.h>
#include <stdlib.h> //used for syscal : exit();

#define DEBUG
#define DEV_NAME "/dev/ttyS0"
#define BUF_LEN 204 // dzt_protocol's max length.

/*
static int get_msg(int fd, unsigned char *msg, int msg_buf)
{
	int len, msg_tail;
	msg_tail = len = 0; 

	unsigned char buf[BUF_LEN];
	unsigned char tail;
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
*/

void main(int argc, char *argv[])
{
	unsigned char message[PUSH_MSG_LEN];
	int fd =  open_tty(DEV_NAME);
	int msg_len;
	long counter = 0;
	int sleep_timer = 60;
	while(1) {
		msg_len = autogen_push_msg(message);
		write(fd,message,msg_len);
		printf("After %d*%ld=%ld seconds, push:\t",
				sleep_timer,counter,sleep_timer*counter);
		msg_printer_raw(message,msg_len);
		sleep(sleep_timer);
		counter++;
	}
}	
