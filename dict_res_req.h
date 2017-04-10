/*
 *  dict_res_req.h dictionary of reponse for request msg.  
 *  XT-MCU 動中通天線 查詢/餉應 命令
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-04-10 08:50
 */

#ifndef DICT_REQ_RES_H
#define DICT_REQ_RES_H

#include "protocol.h"

extern struct cmd_param get_req_res_param(unsigned const char cmd_req);

#endif
