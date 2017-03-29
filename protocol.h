/*
 *  protocol.h
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-28 17:09
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H

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

extern struct dzt_protocol init_dzt_protocol(const char* msg);
extern void dzt_proto_printer( struct dzt_protocol dztp ) ;

#endif
