/*
 *  protocol.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-03-25 16:15
 */
#include "protocol.h"

extern struct dzt_protocol init_dzt_protocol( const char *msg )
{
	struct dzt_protocol dztp;
	/* encapsulation the msg to protocol struct. */
	dztp.head  = msg[0];
	dztp.length = msg[1];
	dztp.addr   = msg[2];
	dztp.cmd    = msg[3];
	dztp.param.len = (int)dztp.length - 3;
	int i = dztp.param.len ;
	int j = 0;
	while ( j < i ) {
		dztp.param.param[j] = msg[4+j];
		j++;
	}
	dztp.checkout = msg[4+i];//TODO: CRC
	dztp.end = msg[4+i+1];

	return dztp;
}

extern void dzt_proto_printer( struct dzt_protocol dztp ) 
{
	printf("%c ", dztp.head);
	printf("%.4x ", dztp.length);
	printf("%.4x ", dztp.addr);
	printf("%.4x ", dztp.cmd);

	int i = dztp.param.len;
	int j = 0 ;
	while ( j < i ) {
		printf("%.4x ", dztp.param.param[j]);
		j++;
	}

	printf("%.4x ", dztp.checkout);
	printf("%c", dztp.end);
	printf("\n");
}
