//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP会话类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef BASE_NETWORK_TCP_SESSION_H
#define BASE_NETWORK_TCP_SESSION_H

#include "boost/shared_ptr.hpp"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
#include "DataStruct/FIFOQueue.h"
#include "ASIO/TCPSender.h"
using TCPSender = base::network::TCPSender;
using TCPSenderPtr = boost::shared_ptr<TCPSender>;
#include "ASIO/TCPReceiver.h"
using TCPReceiver = base::network::TCPReceiver;
using TCPReceiverPtr = boost::shared_ptr<TCPReceiver>;

namespace base
{
	namespace network
	{
		class TCPSession
		{
		public:
			TCPSession(boost::asio::ip::tcp::socket* s = nullptr);
			virtual ~TCPSession(void);

		public:
			//启动会话
			//@Return : 错误码
			//@Comment : 会话启动会自动开始数据接收
			virtual int startSession(void);

			//停止会话
			//@Return : 错误码
			virtual int stopSession(void);

			//发送数据
			//@pkt : 数据包
			//@Return : 错误码
			int sendData(StreamPacket* pkt = nullptr);

			//获取会话状态
			//@Return : 会话状态
			inline const bool valid(void) const
			{
				return !stopped;
			}

			//数据发送通知
			//@e : 错误码
			//@bytes : 数据大小
			virtual void sentdDataNotification(
				const boost::system::error_code e, 
				const int bytes = 0);

			//数据读取通知
			//@data : 数据内容
			//@bytes : 数据大小
			virtual void receivedDataNotification(
				const unsigned char* data = nullptr, 
				const int bytes = 0);

			//数据读取异常通知
			//@e : 错误码
			virtual void receivedExceptionNotification(
				const boost::system::error_code e);

			//数据读取超时通知
			virtual void receivedExpiredNotification(void);

		private:
			boost::asio::ip::tcp::socket* so;
			//避免不断创建发送和接收器
			TCPSenderPtr senderPtr;
			TCPReceiverPtr receiverPtr;
			//为避免底层发送数据缓存被写满导致溢出
			//会话端使用发送队列保证数据一个接一个发送
			FIFOQueue<StreamPacket*> streamPacketGroup;
			bool stopped;
		};//class TCPSession
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TCP_SESSION_H
