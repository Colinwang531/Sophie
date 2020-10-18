//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-27
//		Description :					������ֵ����
//
//		History:						Author									Date										Description
//										������									2020-04-27									����
//

#ifndef BASE_ERROR_H
#define BASE_ERROR_H

//������
static const int eSuccess = 0;
static const int eInvalidParameter = -1;
static const int eBadOperate = -2;
static const int eBadNewObject = -3;
static const int eBadNewThread = -4;
static const int eOutOfRange = -5;
static const int eNotSupport = -100;
static const int eObjectExisted = -101;
static const int eObjectNotExist = -102;
//������
static const int eBadNewSocket = -1000;
static const int eBadBind = -1001;
static const int eBadConnect = -1002;
static const int eBadClose = -1003;
static const int eBadSend = -1004;
static const int eBadRecv = -1005;
//SDK������
static const int eBadInitSDK = -2000;
static const int eBadCleanupSDK = -2001;
static const int eBadLoginDevice = -2002;
static const int eBadLogoutDevice = -2003;
//Э����
static const int eEmptyProtocolField = -3000;
static const int eBadParseProtocol = -3001;
static const int eBadPackProtocol = -3002;
//�ļ���
static const int eBadOpenFile = -4000;
static const int eBadCloseFile = -4001;
static const int eBadWriteData = -4002;

#endif//BASE_ERROR_H
