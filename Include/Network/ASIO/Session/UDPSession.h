//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-14
//		Description :					UDP�Ự��
//
//		History:						Author									Date										Description
//										������									2020-12-14									����
//

#ifndef FRAMEWORK_NETWORK_ASIO_UDP_SESSION_H
#define FRAMEWORK_NETWORK_ASIO_UDP_SESSION_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
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
			typedef boost::function<void(const int, const int)> AfterUDPSendResultCallback;

			//���ս���ص�
			//@_1 : ������
			//@_2 : �������ݻ���
			//@_3 : �������ݴ�С
			//@_4 : ���Ͷ�IPv4��ַ
			//@_5 : ���Ͷ˶˿ں�
			typedef boost::function<void(const int, const void*, const int, const char*, const unsigned short)> AfterUDPReceiveResultCallback;

			class UDPSession
				: public boost::enable_shared_from_this<UDPSession>
			{
			public:
				UDPSession(
					boost::asio::ip::udp::socket s,
					AfterUDPSendResultCallback scb = nullptr,
					AfterUDPReceiveResultCallback rcb = nullptr);
				virtual ~UDPSession(void);

			public:
				//��������
				//@s : socket���
				//@data : ����
				//@bytes : ���ݴ�С
				//@ipv4 : ���ն�IPv4��ַ
				//@port : ���ն˶˿ں�
				//@Return : ������
				virtual int send(
					const void* data = nullptr,
					const int bytes = 0,
					const char* ipv4 = nullptr,
					const unsigned short port = 0);

				//��������
				//@Return : ������
				virtual int receive(void);

			private:
				//�Ự��ʼ��
				void init(void);
				//�Ự����ʼ��
				void uninit(void);

			private:
				boost::asio::ip::udp::socket so;
				AfterUDPSendResultCallback s_callback;
				AfterUDPReceiveResultCallback r_callback;
				void* buffer;
				int bufsize;
			};//class UDPSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_UDP_SESSION_H
