//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					TCPĿ�ĻỰ��
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//

#ifndef FRAMEWORK_NETWORK_TCP_TARGET_SESSION_H
#define FRAMEWORK_NETWORK_TCP_TARGET_SESSION_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"
#include "TCPSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;

namespace framework
{
	namespace network
	{
		namespace asio
		{
			//���ս���ص�
			//@_1 : ������
			//@_2 : �������ݻ���
			//@_3 : �������ݴ�С
			typedef boost::function<void(const char*, const int, TCPSessionPtr)> AfterTargetTCPReceiveResultCallback;

			class TCPTargetSession 
				: public TCPSession, public boost::enable_shared_from_this<TCPTargetSession>
			{
			public:
				TCPTargetSession(
					boost::asio::ip::tcp::socket& so,
					AfterTargetTCPReceiveResultCallback ttrb = nullptr);
				virtual ~TCPTargetSession(void);

			private:
				void afterTCPSendResultCallback(
					const int e = 0,
					const int bytes = 0);
				void afterTCPReceiveResultCallback(
					const int e = 0,
					const void* data = nullptr,
					const int bytes = 0);

			private:
				AfterTargetTCPReceiveResultCallback ttr_callback;
			};//class TCPTargetSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_TCP_TARGET_SESSION_H
