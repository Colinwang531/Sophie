//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP接收器类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef BASE_NETWORK_TCP_RECEIVER_H
#define BASE_NETWORK_TCP_RECEIVER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace base
{
	namespace network
	{
 		typedef boost::function<void(boost::asio::ip::tcp::socket*, const unsigned char*, const int)> AfterGotReadDataNotificationCallback;

		class TCPSession;

		class TCPReceiver : public boost::enable_shared_from_this<TCPReceiver>
		{
		public:
			TCPReceiver(TCPSession* s);
			TCPReceiver(AfterGotReadDataNotificationCallback callback);
			virtual ~TCPReceiver(void);

		public:
			//接收数据
			//@s : SOCKET句柄
			//@timeout : 接收超时间隔,以秒为单位
			//@Return : 错误码值
			void recvData(
				boost::asio::ip::tcp::socket* s = nullptr, 
				const int timeout = 0);

			//获取接收超时状态
			//@Return : 超时状态
			inline const bool recvExpired(void) const
			{
				return expired;
			}

		private:
			void afterReadDataNotificationCallback(
				boost::asio::ip::tcp::socket* s,
				std::size_t bytes, 
				boost::system::error_code e);
			void afterReadDataExpiredNotificationCallback(void);

		private:
			AfterGotReadDataNotificationCallback afterGotReadDataNotificationCallback;
			TCPSession* session;
			bool expired;

			enum
			{
				//1M
				eMaxDataSize = 3 * 1024 * 1024
			};
			unsigned char* dataBuffer;
		};//class TCPReceiver
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TCP_RECEIVER_H
