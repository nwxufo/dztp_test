/*
 *  serial_dev.h
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-24 15:40
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <sys/termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern int open_tty(const char* dev_path);
#endif 
