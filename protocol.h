/*
 *  protocol.h
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-28 17:09
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H

#define BUF_PARAM 199 //MAX protocol's parameter.
struct cmd_param {
	int len;
	unsigned char param[BUF_PARAM];
};

#endif
