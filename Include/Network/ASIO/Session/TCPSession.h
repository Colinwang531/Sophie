//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP�Ự��
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
//

#ifndef FRAMEWORK_NETWORK_ASIO_TCP_SESSION_H
#define FRAMEWORK_NETWORK_ASIO_TCP_SESSION_H

#include "boost/asio.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			//���ͽ���ص�
			//@_1 : ������
			//@_2 : ���ݴ�С
			typedef boost::function<void(const int, const int)> AfterTCPSendResultCallback;

			//���ս���ص�
			//@_1 : ������
			//@_2 : �������ݻ���
			//@_3 : �������ݴ�С
			typedef boost::function<void(const int, const void*, const int)> AfterTCPReceiveResultCallback;

			class TCPSession
			{
			public:
				TCPSession(
					boost::asio::ip::tcp::socket& s,
					AfterTCPSendResultCallback scb = nullptr,
					AfterTCPReceiveResultCallback rcb = nullptr);
				virtual ~TCPSession(void);

			public:
				//��������
				//@data : ����
				//@bytes : ���ݴ�С
				//@Return : ������
				virtual int send(
					const void* data = nullptr,
					const int bytes = 0);

				//��������
				//@Return : ������
				virtual int receive(void);

			private:
				//�Ự��ʼ��
				void init(void);
				//�Ự����ʼ��
				void uninit(void);

			protected:
				boost::asio::ip::tcp::socket so;
				AfterTCPSendResultCallback s_callback;
				AfterTCPReceiveResultCallback r_callback;
				void* buffer;
				int bufsize;
			};//class TCPSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_SESSION_H
