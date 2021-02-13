//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-24
//		Description :					XMQ�ڵ�ģ����
//
//		History:						Author									Date										Description
//										������									2020-12-24									����
//

#ifndef FRAMEWORK_NETWORK_PATTERN_XMQ_PEER_H
#define FRAMEWORK_NETWORK_PATTERN_XMQ_PEER_H

#include "Network/ASIO/Core/Asio.h"
using ASIO = framework::network::asio::ASIO;
#include "Network/ZMQ/OD.h"
using OD = framework::network::zeromq::OD;

namespace framework
{
	namespace network
	{
		namespace pattern
		{
			class XMQPeer 
				: public OD, protected ASIO//, protected UDPSession
			{
			public:
				XMQPeer(
					const std::string name,
					const std::string id);
				virtual ~XMQPeer(void);

			public:
				inline const std::string getName(void) const 
				{
					return peerName;
				}
				inline const std::string getID(void) const
				{
					return peerID;
				}
				int startOD(
					const char* ipv4 = nullptr,
					const unsigned short port = 0) override;
				int stopOD(void) override;
				
// 				virtual void afterUDPSessionExceptionNotification(
// 					const int e = 0) override;
				inline const std::string getLocalIPv4() const
				{
					return localAddr;
				}

			protected:
				//����ע����Ϣ
				//@Return : ��Ϣ
				virtual const std::string buildRegisterMessage(void) = 0;

			private:
				//����IPv4��ַ��ѯ�߳�
				//@ipv4 : UDP������IPv4��ַ
				//@port : UDP�����߶˿ں�
				void queryLocalIPv4Thread(
					const char* ipv4 = nullptr,
					const unsigned short port = 0);

				void afterUDPSendResultNotification(
					const int e = 0,
					const int bytes = 0);
				void afterUDPReceiveResultNotification(
					const int e = 0,
					const void* data = nullptr,
					const int bytes = 0,
					const char* ipv4 = nullptr,
					const unsigned short port = 0);

				//��ʱע���߳�
				//@ipv4 : UDP������IPv4��ַ
				//@port : UDP�����߶˿ں�
				void sendRegisterMessageThread(void);

			private:
				bool isInit;
				const std::string peerName;
				const std::string peerID;
				std::string xmqAddr;
				unsigned short xmqPort;
				int registerSequenceNumber;
				std::string localAddr;
			};//class XMQPeer
		}//namespace pattern
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_PATTERN_XMQ_PEER_H
