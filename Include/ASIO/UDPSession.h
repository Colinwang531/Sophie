/****************************************************************************************************************************************************
Copyright :				@2017, HHJT, All Rights Reserved

Author :					王科威
E-mail :					wangkw531@gmail.com
Date :						2017-06-21
Description :			UDP会话类
								1.会话使用异步方式发送和接收数据
								2.会话必须使用共享指针方式创建

History:					Author									Date												Description
								王科威									2017-06-21								创建
****************************************************************************************************************************************************/


#ifndef UDP_SESSION_H
#define UDP_SESSION_H

#include "Defs.h"

namespace network_1
{
	class UDPSession : public boost::enable_shared_from_this<UDPSession>
	{
	public:
		UDPSession(const UDPSocket* so = NULL);
		virtual ~UDPSession(void);

		//功能 :			启动UDP会话数据交互,该方法由调用者负责实现
		//
		//参数 :	
		//
		//返回值 :		错误码
		virtual int startSession(void) = 0;

		//功能 :			停止UDP会话数据交互
		//
		//参数 :	
		//
		//返回值 :
		virtual void stopSession(void);

		//功能 :			绑定UDP会话实例的数据接收端口
		//
		//参数 :			portNum							[in]								端口号
		//
		//返回值 :		错误码
		virtual int bindReadPort(const boost::uint16_t portNum = gInvalidPort);

		//功能 :			打开一个异步数据读取
		//
		//参数 :	
		//
		//返回值 :		错误码
		virtual int asyncRead(void);

		//功能 :			由调用者实现的UDP异步数据读取事件捕获
		//					调用者必须在处理完读取的数据后调用此基类方法，该方法负责重新打开一个异步数据读取，否则该会话将不能再读取其他交互数据
		//
		//参数 :			udpSenderPtr						[in]							远程数据发送端信息
		//					streamBufPtr						[in]							流数据缓存
		//					streamBufSize						[in]							流数据大小
		//					errorCode							[in]							读取事件错误码
		//
		//返回值 :		错误码
		virtual int asyncReadEventNotify(UDPEndpointPtr udpSenderPtr, StreamBufPtr streamBufPtr, const std::size_t streamBufSize = 0, const ErrorCode& errorCode = ErrorCode());

		//功能 :			打开一个异步数据发送
		//
		//参数 :			address								[in]							远程接收方IP地址,版本为IPV4
		//					portNum								[in]							远程接收方端口号
		//					streamBuf							[in]							流数据缓存
		//					streamBufSize						[in]							流数据大小
		//
		//返回值 :		错误码
		virtual int asyncWrite(const char* address = NULL, const boost::uint16_t portNum = gInvalidPort, const char* streamBuf = NULL, const boost::uint32_t streamBufSize = 0);

		//功能 :			由调用者实现的TCP异步数据发送事件捕获
		//
		//参数 :			streamBufPtr									[in]					流数据缓存
		//					streamBufSize								[in]					流数据大小
		//					errorCode										[in]					读取事件错误码
		//
		//返回值 :		错误码
		virtual int asyncWriteEventNotify(StreamBufPtr streamBufPtr, const std::size_t streamBufSize = 0, const ErrorCode& errorCode = ErrorCode());

	protected:
		UDPSocket* udpSocket;
	};//class UDPSession
}//namespace network_1

#endif//UDP_SESSION_H
