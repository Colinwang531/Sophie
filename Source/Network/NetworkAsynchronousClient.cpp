#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"
#include "Error.h"
#include "Protocol/MessageDefs.h"
#include "Protocol/Alarm/AlarmCodec.h"
#include "Protocol/Algorithm/AlgorithmCodec.h"
#include "Protocol/Component/ComponentCodec.h"
using ComponentParser = base::protocol::ComponentParser;
using ComponentPacker = base::protocol::ComponentPacker;
#include "Protocol/Crew/CrewCodec.h"
#include "Protocol/Device/DeviceCodec.h"
#include "Protocol/Status/StatusCodec.h"
#include "Protocol/MessageCodec.h"
using MessageParser = base::protocol::MessageParser;
using MessagePacker = base::protocol::MessagePacker;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "Time/XTime.h"
using base::time::Time;
#include "MessageQueue/AsynchronousSender.h"
using AsynchronousSender = mq::module::AsynchronousSender;
#include "Network/NetworkAsynchronousClient.h"

namespace base
{
	namespace network
	{
		NetworkAsynchronousClient::NetworkAsynchronousClient() : AsynchronousClient(), isSigned{ false }
		{}

		NetworkAsynchronousClient::~NetworkAsynchronousClient()
		{}

		int NetworkAsynchronousClient::startPoller()
		{
			int e{ AsynchronousClient::startPoller() };

			if (eSuccess == e)
			{
				e = createNewSigninAndHeartbeatWorkerThread();
			}

			return e;
		}

		void NetworkAsynchronousClient::afterClientPollMessage(
			const char* data /* = nullptr */, 
			const unsigned int databytes /* = 0 */)
		{
			//����MSG��װ���ݻ�ȡ��Ϣ���ͺ�����
			MessageParser parser;
			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(parser.createNewPacket()) };

			if (eSuccess == parser.unpackMessage(data, mp) && mp)
			{
				switch (mp->type)
				{
					case MessageType::MESSAGE_TYPE_ALARM:
					{
						parseAlarmMessage(mp);
						break;
					}
					case MessageType::MESSAGE_TYPE_ALGORITHM:
					{
						parseAlgorithmMessage(mp);
						break;
					}
					case MessageType::MESSAGE_TYPE_COMPONENT:
					{
						parseComponentMessage(mp);
						break;
					}
					case MessageType::MESSAGE_TYPE_CREW:
					{
						parseCrewMessage(mp);
						break;
					}
					case MessageType::MESSAGE_TYPE_DEVICE:
					{
						parseDeviceMessage(mp);
						break;
					}
					case MessageType::MESSAGE_TYPE_STATUS:
					{
						parseStatusMessage(mp);
						break;
					}
					case MessageType::MESSAGE_TYPE_USER:
					{
						parseUserMessage(mp);
						break;
					}
					default:
					{
						parseUnknownMessage();
						break;
					}
				}
			}
			else
			{
				unpackMessageFailure();
			}

			//������Ϣ��ʵ��
			parser.destroyPacket(mp);
		}

		void NetworkAsynchronousClient::unpackMessageFailure() {}
		void NetworkAsynchronousClient::parseUnknownMessage() {}
		void NetworkAsynchronousClient::parseAlarmMessage(void* pkt /* = nullptr */) {}
		void NetworkAsynchronousClient::parseAlgorithmMessage(void* pkt /* = nullptr */) {}

		void NetworkAsynchronousClient::parseComponentMessage(void* pkt /* = nullptr */)
		{
			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
			ComponentResponse* cr{ reinterpret_cast<ComponentResponse*>(mp->packet) };
			isSigned = (0 == cr->result ? true : false);
		}

		void NetworkAsynchronousClient::parseCrewMessage(void* pkt /* = nullptr */) {}
		void NetworkAsynchronousClient::parseDeviceMessage(void* pkt /* = nullptr */) {}
		void NetworkAsynchronousClient::parseStatusMessage(void* pkt /* = nullptr */) {}
		void NetworkAsynchronousClient::parseUserMessage(void* pkt /* = nullptr */) {}

		int NetworkAsynchronousClient::createNewSigninAndHeartbeatWorkerThread()
		{
			void* t{
				ThreadPool::get_mutable_instance().createNewThread(
					boost::bind(&NetworkAsynchronousClient::signinAndHeartbeatThreadProc, this)) };
			return t ? eSuccess : eBadNewThread;
		}

		void NetworkAsynchronousClient::signinAndHeartbeatThreadProc(void)
		{
			//ÿ���30��ע�������һ��

			long long lastTimestamp{ 0 };

			while (!stopped)
			{
				const long long nowTimestamp{ Time().tickcount() };

				if (30000 < nowTimestamp - lastTimestamp)
				{
					if (isSigned)
					{
						sendSigninOrHeartbeatRequestMessage(getClientID());
					} 
					else
					{
						sendSigninOrHeartbeatRequestMessage();
					}

					lastTimestamp = nowTimestamp;
				}

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
		}

		const long long NetworkAsynchronousClient::getRequestSequenceNumber()
		{
			WriteLock wl{ mtx };
			const long long currentRequestSequenceNumber{ requestSequence };
			//������Ϣ�����к�ʹ������ֵ
			requestSequence += 2;
			return currentRequestSequenceNumber;
		}

		int NetworkAsynchronousClient::sendSigninOrHeartbeatRequestMessage(const char* clientID /* = nullptr */)
		{
			int e{ eBadPackProtocol };
			std::string cid;
			
			if (clientID)
			{
				cid.append(clientID);
			}

			ComponentInfo ci{
				static_cast<ComponentType>(getClientType()), Time().tickcount(), cid, getClientName() };
			const std::string req{
				MessagePacker().packMessage(
					static_cast<int>(MessageType::MESSAGE_TYPE_COMPONENT), 
					getRequestSequenceNumber(), 
					//��¼��������ʹ��ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ����
					ComponentPacker().packComponent(static_cast<int>(ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ), 0, &ci)) };

			if (!req.empty())
			{
				AsynchronousSender as;
				unsigned int sentBytes{ as.sendData(dealer, req.c_str(), req.length()) };
				e = 0 < sentBytes ? eSuccess : eBadSend;
			}

			return e;
		}
	}//namespace network
}//namespace base
