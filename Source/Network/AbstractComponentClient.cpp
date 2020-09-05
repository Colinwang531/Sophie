#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include "boost/checked_delete.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"
#include "Error.h"
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
#include "Protocol/Component/ComponentPhrase.h"
using CommandPacker = base::protocol::CommandPacker;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "Time/XTime.h"
using base::time::Time;
#include "MessageQueue/AsynchronousSender.h"
using AsynchronousSender = mq::module::AsynchronousSender;
#include "Network/AbstractComponentClient.h"

namespace base
{
	namespace network
	{
		AbstractComponentClient::AbstractComponentClient()
			: AbstractMQClient(), requestSequence{ 1 }
		{}

		AbstractComponentClient::~AbstractComponentClient()
		{}

		int AbstractComponentClient::startPoller()
		{
			int e{ AbstractMQClient::startPoller() };

			if (eSuccess == e)
			{
				void* threadID{
					ThreadPool::get_mutable_instance().createNewThread(
						boost::bind(&AbstractComponentClient::checkAbstractComponentClientOnlineThreadProc, this)) };
				e = threadID ? eSuccess : eBadNewThread;
			}

			return e;
		}

		void AbstractComponentClient::checkAbstractComponentClientOnlineThreadProc(void)
		{
			//每间隔30秒注册或心跳一次

			long long lastTimestamp{ 0 };

			while (!stopped)
			{
				const long long nowTimestamp{ Time().tickcount() };

				if (0 == lastTimestamp || 30000 < nowTimestamp - lastTimestamp)
				{
					sendAbstractComponentClientOnlineRequestMessage();
					lastTimestamp = nowTimestamp;
				}

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
		}

		const long long AbstractComponentClient::getRequestSequenceNumber()
		{
			WriteLock wl{ mtx };
			const long long currentRequestSequenceNumber{ requestSequence };
			//请求消息的序列号使用奇数值
			requestSequence += 2;
			return currentRequestSequenceNumber;
		}

		int AbstractComponentClient::sendAbstractComponentClientOnlineRequestMessage()
		{
			int e{ eBadNewObject };
			MessagePacket mp(
				base::packet::PacketType::PACKET_TYPE_COMPONENT,
				static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ));
			AbstractComponent* ac{
					new(std::nothrow) AbstractComponent(
						static_cast<base::component::ComponentType>(getAbstractComponentClientType())) };

			if (ac)
			{
				ac->setComponentID(getAbstractComponentClientID());
				ac->setComponentName(getAbstractComponentClientName());
				mp.setPacketSequence(getRequestSequenceNumber());
				mp.setPayloadData(ac);

				const std::string command{ CommandPacker().packPacketToArray(&mp) };
				if (!command.empty())
				{
					const std::string delimiter{ " " };
					AsynchronousSender as;
					const unsigned int sentBytes{
						as.sendData(dealer, delimiter.c_str(), delimiter.length(), true) +
						as.sendData(dealer, command.c_str(), command.length()) };
					e = 0 < sentBytes ? eSuccess : eBadSend;
				}
				else
				{
					e = eBadPackProtocol;
				}
			}

			return e;
		}
	}//namespace network
}//namespace base
