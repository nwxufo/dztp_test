/*
 *  response.h
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-28 17:16
 */
#ifndef RESPONSE_H
#define RESPONSE_H

#include "protocol.h"

struct dzt_protocol{
	unsigned char head;
	unsigned char length;
	unsigned char addr; 
	unsigned char cmd;
	struct cmd_param param;
	unsigned char checkout;
	unsigned char end;
};

enum recv_cmd_type  {RECOVERY,VERSION_CHECK,SETTING,CHECKOUT};
struct response_struct {
	enum recv_cmd_type type;
	int fd;		//tty devices file descriptor
	struct dzt_protocol dztp;
};
/*
extern void msg_response_recovery(struct response_struct res_obj);
extern void msg_response_version(struct response_struct res_obj);
extern void msg_response_setting(struct response_struct res_obj);
extern void msg_response_checkout(struct response_struct res_obj);

extern void (*msg_response_tbl[])(struct response_struct);
#define msg_response(z) msg_response_tbl[z.type](z)
*/
extern void msg_printer_raw(const unsigned char* msg, int len);
extern void  msg_processor(const int fd, const unsigned char* msg);
#endif
