//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-24
//		Description :					XMQ节点模型类
//
//		History:						Author									Date										Description
//										王科威									2020-12-24									创建
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
				//构建注册消息
				//@Return : 消息
				virtual const std::string buildRegisterMessage(void) = 0;

			private:
				//本地IPv4地址查询线程
				//@ipv4 : UDP接收者IPv4地址
				//@port : UDP接收者端口号
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

				//定时注册线程
				//@ipv4 : UDP接收者IPv4地址
				//@port : UDP接收者端口号
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
