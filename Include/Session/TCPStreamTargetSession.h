//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					流媒体目的会话类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//

#ifndef BASE_NETWORK_STREAM_TARGET_SESSION_H
#define BASE_NETWORK_STREAM_TARGET_SESSION_H

#include "boost/function.hpp"
#include "ASIO/TCPSession.h"
using TCPSession = base::network::TCPSession;

namespace base
{
	namespace network
	{
		class TCPStreamTargetSession : public TCPSession
		{
		public:
			TCPStreamTargetSession(
				boost::asio::ip::tcp::socket* s = nullptr);
			virtual ~TCPStreamTargetSession(void);

		protected:
			void receivedDataNotification(
				const unsigned char* data = nullptr,
				const int bytes = 0) override;
		};//class TCPStreamTargetSession
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_STREAM_TARGET_SESSION_H
