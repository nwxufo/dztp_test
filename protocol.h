/*
 *  protocol.h
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-28 17:09
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <stdio.h>

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

extern unsigned char get_crc(const unsigned char* msg, const int len);
extern size_t dzt_protocol_to_msg(struct dzt_protocol* const dztpp, unsigned char msg[]);
extern void msg_printer_raw(const unsigned char* msg, int len);
#endif
