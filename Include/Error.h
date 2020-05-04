//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-27
//		Description :					错误码值定义
//
//		History:						Author									Date										Description
//										王科威									2020-04-27									创建
//

#ifndef ERROR_H
#define ERROR_H

//公共类
static const int eSuccess = 0;
static const int eInvalidParameter = 1;
static const int eBadOperate = 2;
static const int eBadNewObject = 3;
static const int eBadNewThread = 4;
//网络类
static const int eBindPortFail = 1000;
static const int eBindInprocFail = 1001;
static const int eBadNewSocket = 1002;
static const int eBadConnect = 1003;
static const int eBadClose = 1004;

#endif//ERROR_H
