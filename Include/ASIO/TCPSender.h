//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP��������
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
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
			//��������
			//@s : SOCKET���
			//@data : ��������
			//@bytes : ���ݴ�С
			//@Return : ������ֵ
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
			//�������ݵ�ַ�ͷ��ͳ���
			//һ�η��Ͳ����������ݱ����������
			unsigned char* dataBuffer;
			int dataBufferBytes;
			int sentDataBytes;
		};//class TCPSender
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TCP_SENDER_H
