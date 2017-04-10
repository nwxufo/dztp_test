/*
 *  dict_res_req.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-04-10 09:13
 */

#include "dict_res_req.h"
#include <stdio.h>
#include "protocol.h"

#define REQ_RES_TAG 0XA0
#define REQ_RES_NUM 16


static struct cmd_param param_res_req[REQ_RES_NUM] = {
{//0
	.len = 8,
	.param = "2170.000",
},
{//1
	.len = 41+5,
	.param = "2710.000,2175.000,2180.000,2190.000,2200.000,",
	.param[40+5] = 0x05,
			
},
{//2
	.len = 13+4+1,
	.param = "34.00,N,108.00,E,",
	.param[13+4] = 0x01,
},
{//3
	.len = 1,
	.param = { 0x03 },
},
{//4
	.len = 4,
	.param = "-20.0",
},
{//5
	.len = 3,
	.param = { "40.0" },
},
{//6
	.len = 3,
	.param = "60.0",
},
{//7
	.len = 4,
	.param = "+25.0",
},
{//8
	.len = 5,
	.param = "0.001",
},
{ //故障代碼9
	.len = 1,
	.param = {0xE0},
},
{ //undefine
	.len = 0,
	//.param = NULL,
},
{//0xab
	.len = 11+1,
	.param = {"180.00,50.00"},
},
{//0xac
	.len = 0,
	//.param = NULL,
},
{
	.len = 0,
	//.param = NULL,
},
{
	.len = 0,
	//.param = NULL,
},
{//0xaf
	.len = 36+30+14,
	.param = "1995.00,2170.000,34.00,N,108.00,E,X,X,-20.0,40.0,60.0,+25.0,0.001,X,180.00,50.00", 
	.param[34] = 0x01,
	.param[36] = 0X03,
	.param[66] = 0xe0,
},
};

struct cmd_param get_req_res_param(unsigned const char cmd_req)
{
	int index = (int) (cmd_req - REQ_RES_TAG);
		if( index == 0xAA ||
		    index == 0xAC ||
		    index == 0xAD ||
		    index == 0xAE ) 
			perror ("recieved undefined request cmd !");

	struct cmd_param tmp;
	if (index == 15) { 	//test 系統狀態查詢。
		printf("發送的系統狀態參數:\n");
		tmp = param_res_req[index];
		int i;
		for(i=0; i < tmp.len; i++) {
			if(i==34||i==36||i==66) {
				printf("0x%.2x",tmp.param[i]);
				continue;
			}
			printf("%c",tmp.param[i]);
		}
		printf("\n");
	}

	return param_res_req[index];
}
