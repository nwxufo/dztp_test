/*
 *  serial_dev.c used to open serial device.
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-23 16:18
 */

static void init_serial(int fd)
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
	
	//return fd;
}

extern int open_tty(const char *dev_path)
{
	int file_desc;
	file_desc = open(dev_path, O_RDWR | O_NOCTTY /*|O_NONBLOCK*/);
		if ( file_desc < 0 ) {
			printf(" Cannot open device %s! \n",dev_path);
			printf(" 提示，设备读写需要权限！\n");
			exit( -1 );
		}

	init_serial(file_desc);

	return file_desc;
}
