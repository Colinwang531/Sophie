//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP发送器类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef BASE_NETWORK_TCP_SENDER_H
#define BASE_NETWORK_TCP_SENDER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace base
{
	namespace network
	{
		class TCPSession;

		class TCPSender : public boost::enable_shared_from_this<TCPSender>
		{
		public:
			TCPSender(TCPSession* s = nullptr);
			virtual ~TCPSender(void);

		public:
			//发送数据
			//@s : SOCKET句柄
			//@data : 数据内容
			//@bytes : 数据大小
			//@Return : 错误码值
			int sendData(
				boost::asio::ip::tcp::socket* s = nullptr, 
				const unsigned char* data = nullptr, 
				const int bytes = 0);

		private:
			void afterSentDataNotificationCallback(
				boost::asio::ip::tcp::socket* s,
				std::size_t bytes,
				boost::system::error_code e);

		private:
			TCPSession* session;
			//保存数据地址和发送长度
			//一次发送不完整的数据必须继续发送
			unsigned char* dataBuffer;
			int dataBufferBytes;
			int sentDataBytes;
		};//class TCPSender
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TCP_SENDER_H
