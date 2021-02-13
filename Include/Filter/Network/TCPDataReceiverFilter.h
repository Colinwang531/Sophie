//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					�����������
//
//		History:						Author									Date										Description
//										������									2020-09-23									����
//

#ifndef FRAMEWORK_MULTIMEDIA_TCP_DATA_RECEIVER_FILTER_H
#define FRAMEWORK_MULTIMEDIA_TCP_DATA_RECEIVER_FILTER_H

#include "Filter/Filter.h"
#include "Network/ASIO/Session/TCPSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;

namespace framework
{
	namespace multimedia
	{
		class TCPDataReceiverFilter : public Filter
		{
		public:
			TCPDataReceiverFilter(void);
			virtual ~TCPDataReceiverFilter(void);

		public:
			//����
			//@url : ��URL
			//@so : socket
			//@Return : ������
			int start(
				const std::string url, 
				boost::asio::ip::tcp::socket& so);

		private:
			void afterTCPSendResultCallback(
				const int e = 0, 
				const int bytes = 0);
			void afterTCPReceiveResultCallback(
				const int e = 0, 
				const void* data = nullptr, 
				const int bytes = 0);

		private:
			TCPSessionPtr sp;
		};//class TCPDataReceiverFilter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_TCP_DATA_RECEIVER_FILTER_H
