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
		NetworkAsynchronousClient::NetworkAsynchronousClient() 
			: AsynchronousClient(), requestSequence{ 1 }
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
			const void* data /* = nullptr */, 
			const unsigned int databytes /* = 0 */)
		{
			//解析MSG封装数据获取消息类型和内容
			MessageParser parser;
			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(parser.createNewPacket()) };

			if (eSuccess == parser.unpackMessage(data, databytes, mp) && mp)
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

			//销毁消息包实例
			parser.destroyPacket(mp);
		}

		int NetworkAsynchronousClient::createNewSigninAndHeartbeatWorkerThread()
		{
			void* t{
				ThreadPool::get_mutable_instance().createNewThread(
					boost::bind(&NetworkAsynchronousClient::signinAndHeartbeatThreadProc, this)) };
			return t ? eSuccess : eBadNewThread;
		}

		void NetworkAsynchronousClient::signinAndHeartbeatThreadProc(void)
		{
			//每间隔30秒注册或心跳一次

			long long lastTimestamp{ 0 };

			while (!stopped)
			{
				const long long nowTimestamp{ Time().tickcount() };

				if (0 == lastTimestamp || 30000 < nowTimestamp - lastTimestamp)
				{
					sendSigninOrHeartbeatRequestMessage();
					lastTimestamp = nowTimestamp;
				}

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
		}

		const long long NetworkAsynchronousClient::getRequestSequenceNumber()
		{
			WriteLock wl{ mtx };
			const long long currentRequestSequenceNumber{ requestSequence };
			//请求消息的序列号使用奇数值
			requestSequence += 2;
			return currentRequestSequenceNumber;
		}

		int NetworkAsynchronousClient::sendSigninOrHeartbeatRequestMessage()
		{
			int e{ eBadPackProtocol };
			ComponentInfo ci{
				static_cast<ComponentType>(getClientType()), 
				Time().tickcount(), 
				"",
				getClientID(), 
				getClientName() };
			const std::string req{
				MessagePacker().packMessage(
					static_cast<int>(MessageType::MESSAGE_TYPE_COMPONENT), 
					getRequestSequenceNumber(), 
					//注册和心跳都使用ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ类型
					ComponentPacker().packComponent(static_cast<int>(ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ), 0, &ci)) };

			if (!req.empty())
			{
				const std::string delimiter{" "};
				AsynchronousSender as;
				const unsigned int sentBytes{ 
					as.sendData(dealer, delimiter.c_str(), delimiter.length(), true) +
					as.sendData(dealer, req.c_str(), req.length()) };
				e = 0 < sentBytes ? eSuccess : eBadSend;
			}

			return e;
		}
	}//namespace network
}//namespace base
