//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					流媒体源会话类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//

#ifndef BASE_NETWORK_STREAM_SOURCE_SESSION_H
#define BASE_NETWORK_STREAM_SOURCE_SESSION_H

#include "boost/shared_ptr.hpp"
#include "DataStruct/Vector.h"
#include "ASIO/TCPSession.h"
using TCPSession = base::network::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;

namespace base
{
	namespace network
	{
		class TCPStreamSourceSession : public TCPSession
		{
		public:
			TCPStreamSourceSession(
				boost::asio::ip::tcp::socket* s = nullptr);
			virtual ~TCPStreamSourceSession(void);

		public:
			//添加目的端会话
			//@s : SOCKET句柄值
			//@Return : 错误码
			int addStreamTargetSession(boost::asio::ip::tcp::socket* s = nullptr);

		protected:
			void receivedDataNotification(
				const unsigned char* data = nullptr, 
				const int bytes = 0) override;
			void receivedExceptionNotification(
				const boost::system::error_code e) override;
			void receivedExpiredNotification(void) override;

		private:
			//目的会话集合
			Vector<TCPSessionPtr> streamTargetSessionGroup;
		};//class TCPServerSession
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_STREAM_SOURCE_SESSION_H
