/*
 *  test_ttyS0.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2016-12-07 15:11
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/termios.h>

#define DEBUG
#define DEV_NAME "/dev/ttyS0"
#define BUF_LEN 204 // dzt_protocol's max length.

int init_serial(int fd)
{
	struct termios ios;
	tcgetattr(fd, &ios);
		ios.c_cflag = 0;
		ios.c_cflag |=  (CREAD | CLOCAL);
		ios.c_cflag &= ~CRTSCTS;
		ios.c_iflag = 0;
		ios.c_oflag = 0;
		ios.c_lflag = 0;
		ios.c_cc[VMIN] = 1;
		ios.c_cc[VTIME] = 10;
		ios.c_cflag &= ~CSIZE;
		ios.c_cflag |= CS8;
		ios.c_cflag &= ~CSTOPB;
		ios.c_cflag &= ~PARENB;
		ios.c_iflag &= ~INPCK;
	int ret = tcsetattr ( fd, TCSANOW, &ios );
		tcflush(fd, TCIOFLUSH);

	struct termios ios1;
	tcgetattr ( fd, &ios1 );
		cfmakeraw(&ios1);
		cfsetispeed(&ios1, B115200);
		cfsetospeed(&ios1, B115200);

	tcsetattr( fd, TCSANOW, &ios1);
		ret = tcflush(fd, TCIOFLUSH);
	
	return fd;
}

static void  msg_processor(const char* msg)
{
	unsigned char c;
	int index = 1;

	c = msg[0];
	printf ("< ");

	while ( ( c = msg[index] ) != '>' ) {
		printf ("%.4x ", c);
//		printf ("%.4x ", c&0xff);
		++index;
	}
	printf (">\n");

}

int main(int argc, char *argv[])
{
	int file_desc, ret_val;
	char message[BUF_LEN];
	file_desc = open(DEV_NAME, O_RDWR | O_NOCTTY /*|O_NONBLOCK*/);
		if ( file_desc < 0 ) {
			printf(" Cannot open device %s! \n",DEV_NAME);
			printf(" 提示，设备读写需要权限！\n");
			return -1;
		}
	init_serial(file_desc);

	while(1){
		ret_val = read(file_desc,message,BUF_LEN);

		//printf("message recieved :%s\n", message);
		msg_processor(message);
	}

	return ret_val;

}
