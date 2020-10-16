//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					��ý��Դ�Ự��
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
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
			//���Ŀ�Ķ˻Ự
			//@s : SOCKET���ֵ
			//@Return : ������
			int addStreamTargetSession(boost::asio::ip::tcp::socket* s = nullptr);

		protected:
			void receivedDataNotification(
				const unsigned char* data = nullptr, 
				const int bytes = 0) override;
			void receivedExceptionNotification(
				const boost::system::error_code e) override;
			void receivedExpiredNotification(void) override;

		private:
			//Ŀ�ĻỰ����
			Vector<TCPSessionPtr> streamTargetSessionGroup;
		};//class TCPServerSession
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_STREAM_SOURCE_SESSION_H
