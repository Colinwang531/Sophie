//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-27
//		Description :					公共字段值定义
//
//		History:						Author									Date										Description
//										王科威									2020-04-27									创建
//										王科威									2020-05-13									修改命名规范
//

#ifndef DEFINE_H
#define DEFINE_H

//端口号使用范围
static const unsigned short gMinPortNumber = 5000;
static const unsigned short gMaxPortNumber = 65530;

//进程间通信ID标识
static const char* gInprocCommID = "inproc://AsynchronousServer";

#endif//DEFINE_H
