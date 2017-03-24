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
#define BUF_PARA 199 //MAX protocol's parameter.
struct {
	unsigned char start;
	unsigned char length;
	unsigned char addr; 
	unsigned char cmd;
	unsigned char para[BUF_PARA];
	unsigned char checkout;
	unsigned char end;
}
enum reponse_type { CONTROL, CHECKOUT }
struct reponse_t {
	reponse_type	t;

static void  msg_processor(const char* msg)
{
	unsigned char c;
	int index = 1;

	c = msg[0];
	printf ("< ");

	unsigned char cmd;

	while ( ( c = msg[index] ) != '>' ) {
		printf ("%.4x ", c);
		if ( 3 == index ) {
			msg_response
		}
//		printf ("%.4x ", c&0xff);
		++index;
	}
	printf (">\n");

}

int main(int argc, char *argv[])
{
	int fd, ret_val;
	char message[BUF_LEN];

	const char * dev = DEV_NAME; 
	fd = open_tty(dev);

	while(1){
		ret_val = read(fd,message,BUF_LEN);

		//printf("message recieved :%s\n", message);
		msg_processor(message);
	}

	return ret_val;

}	
