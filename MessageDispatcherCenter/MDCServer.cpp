#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/uuid/uuid_generators.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "Protocol/CommandPhrase.h"
using CommandParser = base::protocol::CommandParser;
#include "Protocol/Component/ComponentPhrase.h"
#include "Protocol/Device/DevicePhrase.h"
#include "Protocol/Status/StatusPhrase.h"
#include "Packet/Message/MessagePacket.h"
using AbstractPacket = base::packet::AbstractPacket;
using MessagePacket = base::packet::MessagePacket;
#include "Device/AbstractDevice.h"
using AbstractDevice = base::device::AbstractDevice;
#include "MessageQueue/AsynchronousSender.h"
using AsynchronousSender = mq::module::AsynchronousSender;
#include "Time/XTime.h"
using base::time::Time;
#include "MDCServer.h"

MDCServer::MDCServer() 
	: AsynchronousUpstreamServer(), sailStatus{ true }, autoStatus{ true }
{}

MDCServer::~MDCServer()
{}

int MDCServer::startPoller()
{
	int e{ AsynchronousUpstreamServer::startPoller() };

	if (eSuccess == e)
	{
		//创建服务端心跳业务检测线程
		void* t{
			ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&MDCServer::autoCheckOfflinStatusOfComponentThreadProc, this)) };
		e = t ? eSuccess : eBadNewThread;
	}

	return e;
}

void MDCServer::afterServerPollMessage(
	void* s /* = nullptr */, 
	const void* id /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	const void* delimiter /* = nullptr */, 
	const unsigned int delimiterbytes /* = 0 */, 
	const void* data /* = nullptr */, 
	const unsigned int databytes /* = 0 */)
{
	//Just for testing.
// 	LOG(INFO) << "[ ID:" << idbytes << " ] " << id;
// 	LOG(INFO) << "[ Delimiter:" << delimiterbytes << " ] " << delimiter;
// 	LOG(INFO) << "[ Data:" << databytes << " ] " << data;

	AbstractPacket* pkt{ 
		reinterpret_cast<AbstractPacket*>(
			CommandParser().parseFromArray(data, databytes)) };

	if (pkt)
	{
		switch (pkt->getPacketType())
		{
			case base::packet::PacketType::PACKET_TYPE_ALARM:
			{
//				forwardAlgorithmConfigureMessage(s, data, databytes);
				break;
			}
			case base::packet::PacketType::PACKET_TYPE_ALGORITHM:
			{
				forwardAlgorithmConfigureMessage(s, id, idbytes, pkt, data, databytes);
				break;
			}
			case base::packet::PacketType::PACKET_TYPE_COMPONENT:
			{
				dealWithComponentMessage(s, id, idbytes, pkt);
				break;
			}
			case base::packet::PacketType::PACKET_TYPE_CREW:
			{
				forwardCrewConfigureMessage(s, id, idbytes, pkt, data, databytes);
				break;
			}
			case base::packet::PacketType::PACKET_TYPE_DEVICE:
			{
				forwardDeviceConfigureMessage(s, id, idbytes, pkt, data, databytes);
				break;
			}
			case base::packet::PacketType::PACKET_TYPE_STATUS:
			{
				forwardStatusConfigureMessage(s, id, idbytes, pkt, data, databytes);
				break;
			}
			case base::packet::PacketType::PACKET_TYPE_USER:
			{
				break;
			}
			default:
			{
				LOG(INFO) << "Parse a unknown message from data stream.";
				break;
			}
		}
	}
	else
	{
		LOG(ERROR) << "Parse message from data stream failed.";
	}

	boost::checked_delete(boost::polymorphic_downcast<MessagePacket*>(pkt));
}

void MDCServer::afterUpstreamPollMessage(
	const void* msg /* = nullptr */, 
	const unsigned int bytes /* = 0 */)
{
}

void MDCServer::autoCheckOfflinStatusOfComponentThreadProc()
{
	long long startTime{ Time().tickcount() };

	while (!stopped)
	{
		long long nowTime{ Time().tickcount() };

		//每30秒检测一次
		if (30000 < nowTime - startTime)
		{
			startTime = nowTime;

			std::vector<AbstractComponentPtr> componentPtrs;
			registerComponentGroup.values(componentPtrs);
			for (int i = 0; i != componentPtrs.size(); ++i)
			{
				//超时90秒移除组件实例
				if (90000 < nowTime - componentPtrs[i]->getComponentTimestamp())
				{
					removeRegisterComponent(
						componentPtrs[i]->getComponentID(), 
						static_cast<int>(componentPtrs[i]->getComponentType()));
				}
			}
		}

		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}

void MDCServer::dealWithComponentMessage(
	void* s /* = nullptr */, 
	const void* commID /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	void* pkt /* = nullptr */)
{
	AbstractPacket* ap{ reinterpret_cast<AbstractPacket*>(pkt) };
	const base::protocol::ComponentCommand command{ 
		static_cast<base::protocol::ComponentCommand>(ap->getPacketDataCommandType()) };

	if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == command)
	{
		//组件ID为空表示组件请求注册
		//组件ID不空表示组件请求心跳

		AbstractComponent* ac{ 
			reinterpret_cast<AbstractComponent*>(const_cast<void*>(ap->getPacketData())) };
		std::string componentID{ ac->getComponentID() };

		if (componentID.empty())
		{
			const std::string communicationID{ static_cast<const char*>(commID) };
			ac->setCommunicationID(communicationID);
			//为新注册的组件分配ID标识
			componentID = boost::uuids::to_string(boost::uuids::random_generator()());
			ac->setComponentID(componentID);
			addRegisterComponent(ac);
			replyAddRegisterComponent(
				componentID, componentID.empty() ? eBadOperate : eSuccess, ap->getPacketSequence() + 1, s, commID, idbytes);
		}
		else
		{
			updateRegisterCompnent(ac);
		}
	}
	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == command)
	{
		AbstractComponent* ac{
			reinterpret_cast<AbstractComponent*>(const_cast<void*>(ap->getPacketData())) };
		removeRegisterComponent(
			ac->getComponentID(), 
			static_cast<int>(ac->getComponentType()));
	}
	else if (base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REQ == command)
	{
		std::vector<AbstractComponentPtr> componentPtrs;
		registerComponentGroup.values(componentPtrs);
		replyQueryRegisterComponent(componentPtrs, eSuccess, ap->getPacketSequence() + 1, s, commID, idbytes);
	}
}

void MDCServer::forwardDeviceConfigureMessage(
	void* s /* = nullptr */, 
	const void* commID /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	void* pkt /* = nullptr */, 
	const void* data /* = nullptr */, 
	const unsigned int databytes /* = 0 */)
{
	//设备消息通过消息队列在WEB组件和设备组件之间交互
	//该消息队列只转发消息但不处理

	AbstractPacket* mp{ reinterpret_cast<AbstractPacket*>(pkt) };
	const base::protocol::DeviceCommand command{
		static_cast<base::protocol::DeviceCommand>(mp->getPacketDataCommandType()) };

	if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REQ == command ||
		base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REQ == command ||
		base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REQ == command)
	{
		AbstractDevice* ad{ 
			reinterpret_cast<AbstractDevice*>(const_cast<void*>(mp->getPacketData())) };
		const base::device::DeviceFactory factory{ ad->getDeviceFactory() };
		
		if (base::device::DeviceFactory::DEVICE_FACTORY_HK == factory)
		{
			int e{ 
				forwardConfigureRequestOrResponseMessage(
					s, static_cast<int>(base::component::ComponentType::COMPONENT_TYPE_HKD), data, databytes) };

			if (eSuccess == e)
			{
				LOG(INFO) << "Forward request message of device configure to HK component successfully.";
			} 
			else
			{
				LOG(WARNING) << "Forward request message of device configure to HK component failed, result = " << e << ".";
			}
		}
		else if (base::device::DeviceFactory::DEVICE_FACTORY_DH == factory)
		{
			int e{ 
				forwardConfigureRequestOrResponseMessage(
					s, static_cast<int>(base::component::ComponentType::COMPONENT_TYPE_DHD), data, databytes) };

			if (eSuccess == e)
			{
				LOG(INFO) << "Forward request message of device configure to DH component successfully.";
			}
			else
			{
				LOG(WARNING) << "Forward request message of device configure to DH component failed, result = " << e << ".";
			}
		}
// 		else if (DeviceFactory::DEVICE_FACTORY_ET == dr->embedded.factory)
// 		{
// 		}
		else
		{
			LOG(WARNING) << 
				"Can not forward request message of device configure with unknown factory = " << static_cast<int>(factory) <<
				", type = " << static_cast<int>(ad->getDeviceType()) <<
				", IP = " << ad->getDeviceIPv4Address() <<
				", Port = " << ad->getDevicePortNumber() << ".";
		}
	}
	else if (base::protocol::DeviceCommand::DEVICE_COMMAND_NEW_REP == command ||
		base::protocol::DeviceCommand::DEVICE_COMMAND_DELETE_REP == command ||
		base::protocol::DeviceCommand::DEVICE_COMMAND_MODIFY_REP == command)
	{
		int e{
			forwardConfigureRequestOrResponseMessage(
				s, static_cast<int>(base::component::ComponentType::COMPONENT_TYPE_WEB), data, databytes) };

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward response message of device configure to WEB component successfully.";
		}
		else
		{
			LOG(WARNING) << "Forward response message of device configure to WEB component failed, result = " << e << ".";
		}
	}
}

void MDCServer::forwardStatusConfigureMessage(
	void* s /* = nullptr */, 
	const void* commID /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	void* pkt /* = nullptr */, 
	const void* data /* = nullptr */, 
	const unsigned int databytes /* = 0 */)
{
	AbstractPacket* mp{ reinterpret_cast<AbstractPacket*>(pkt) };
	const base::protocol::StatusCommand command{
		static_cast<base::protocol::StatusCommand>(mp->getPacketDataCommandType()) };

	//由于事先无法得知有哪些设备组件在线,所以每次都必须向全部类型的设备组件转发一次
	//当没有设备组件在线时由服务端进行应答

	if (base::protocol::StatusCommand::STATUS_COMMAND_SET_REQ == command)
	{
		int e{
			forwardConfigureRequestOrResponseMessage(
				s, static_cast<int>(base::component::ComponentType::COMPONENT_TYPE_AI), data, databytes)};

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward request message of device configure successfully.";
		}
		else
		{
			LOG(WARNING) << "Forward request message of device configure failed, result = " << e << ".";
			replyConfigureSetFailedMessage(
				static_cast<int>(base::protocol::StatusCommand::STATUS_COMMAND_SET_REP), mp->getPacketSequence() + 1, s, commID, idbytes);
		}
	}
	else if (base::protocol::StatusCommand::STATUS_COMMAND_SET_REP == command)
	{
		int e{
			forwardConfigureRequestOrResponseMessage(
				s, static_cast<int>(base::component::ComponentType::COMPONENT_TYPE_WEB), data, databytes) };

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward response message of device configure successfully.";
		}
		else
		{
			LOG(WARNING) << "Forward response message of device configure failed, result = " << e << ".";
		}
	}
}

void MDCServer::forwardAlgorithmConfigureMessage(
	void* s /* = nullptr */, 
	const void* commID /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	void* pkt /* = nullptr */, 
	const void* data /* = nullptr */, 
	const unsigned int databytes /* = 0 */)
{
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
	AlgorithmPacket* ap{ reinterpret_cast<AlgorithmPacket*>(mp->packet) };
	const AlgorithmCommand command{ reinterpret_cast<AlgorithmCommand>(ap->command) };

	//由于事先无法得知有几个算法组件在线,所以每次都必须向全部类型的算法组件转发一次
	//当没有算法组件在线时由服务端进行应答

	if (AlgorithmCommand::ALGORITHM_COMMAND_SET_REQ == command)
	{
		int e{
			forwardConfigureRequestOrResponseMessage(
				s, static_cast<int>(ComponentType::COMPONENT_TYPE_AI), data, databytes) };

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward request message of Algorithm configure successfully.";
		}
		else
		{
			LOG(WARNING) << "Forward request message of Algorithm configure failed, result = " << e << ".";
			replyConfigureSetFailedMessage(
				static_cast<int>(AlgorithmCommand::ALGORITHM_COMMAND_SET_REQ) + 1, mp->sequence + 1, s, commID, idbytes);
		}
	}
	else if (AlgorithmCommand::ALGORITHM_COMMAND_SET_REP == command)
	{
		int e{
			forwardConfigureRequestOrResponseMessage(
				s, static_cast<int>(ComponentType::COMPONENT_TYPE_WEB), data, databytes) };

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward response message of Algorithm configure successfully.";
		}
		else
		{
			LOG(WARNING) << "Forward response message of Algorithm configure failed, result = " << e << ".";
		}
	}
}

void MDCServer::forwardAlarmInfoMessage(
	void* s /* = nullptr */,
	const void* data /* = nullptr */,
	const unsigned int databytes /* = 0 */)
{
	int e{
		forwardConfigureRequestOrResponseMessage(
			s, static_cast<int>(ComponentType::COMPONENT_TYPE_ALM), data, databytes) };

	if (eSuccess == e)
	{
		LOG(INFO) << "Forward alarm message of algorithm successfully.";
	}
	else
	{
		LOG(WARNING) << "Forward alarm message of algorithm failed, result = " << e << ".";
	}
}

void MDCServer::forwardCrewConfigureMessage(
	void* s /* = nullptr */,
	const void* commID /* = nullptr */,
	const unsigned int idbytes /* = 0 */,
	void* pkt /* = nullptr */,
	const void* data /* = nullptr */,
	const unsigned int databytes /* = 0 */)
{
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
	CrewPacket* cp{ reinterpret_cast<CrewPacket*>(mp->packet) };
	const CrewCommand command{ reinterpret_cast<CrewCommand>(cp->command) };

	//由于事先无法得知有几个算法组件在线,所以每次都必须向全部类型的算法组件转发一次
	//当没有算法组件在线时由服务端进行应答

	if (CrewCommand::CREW_COMMAND_NEW_REQ == command ||
		CrewCommand::CREW_COMMAND_DELETE_REQ == command ||
		CrewCommand::CREW_COMMAND_MODIFY_REQ == command)
	{
		int e{
			forwardConfigureRequestOrResponseMessage(
				s, static_cast<int>(ComponentType::COMPONENT_TYPE_AI), data, databytes) };

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward request message of crew configure successfully.";
		}
		else
		{
			LOG(WARNING) << "Forward request message of crew configure failed, result = " << e << ".";
			replyConfigureSetFailedMessage(
				static_cast<int>(command) + 1, mp->sequence + 1, s, commID, idbytes);
		}
	}
	else if (CrewCommand::CREW_COMMAND_NEW_REP == command ||
		CrewCommand::CREW_COMMAND_DELETE_REP == command ||
		CrewCommand::CREW_COMMAND_MODIFY_REP == command)
	{
		int e{
			forwardConfigureRequestOrResponseMessage(
				s, static_cast<int>(ComponentType::COMPONENT_TYPE_WEB), data, databytes) };

		if (eSuccess == e)
		{
			LOG(INFO) << "Forward response message of crew configure successfully.";
		}
		else
		{
			LOG(WARNING) << "Forward response message of crew configure failed, result = " << e << ".";
		}
	}
}

void MDCServer::addRegisterComponent(void* c /* = nullptr */)
{
	//通过shared_ptr使实例能自动析构
	boost::shared_ptr<AbstractComponent> acp{ reinterpret_cast<AbstractComponent*>(c) };
	registerComponentGroup.insert(acp->getComponentID(), acp);
	LOG(INFO) << "Add register component type = " << static_cast<int>(acp->getComponentType()) <<
		", ID = " << acp->getComponentID() <<
		", Name = " << acp->getComponentName() <<
		", Timestamp = " << acp->getComponentTimestamp() << ".";
}

int MDCServer::replyAddRegisterComponent(
	const std::string cid, 
	const int result /* = 0 */, 
	const long long sequence /* = 0 */, 
	void* s /* = nullptr */, 
	const void* commID /* = nullptr */, 
	const unsigned int idbytes /* = 0 */)
{
	int e{ s && 0 < sequence && commID && 0 < idbytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AbstractPacket* pkt{ 
			new(std::nothrow) MessagePacket(
				base::packet::PacketType::PACKET_TYPE_COMPONENT, 
				static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP),
				result) };

		if (pkt)
		{
			pkt->setPacketSequence(sequence);
			pkt->setPacketData(const_cast<char*>(cid.c_str()));
			const std::string rep{
				base::protocol::CommandPacker().packPacketToArray(pkt) };
			e = rep.empty() ? eBadPackProtocol : sendServerResponseMessage(s, commID, idbytes, rep.c_str(), rep.length());
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int MDCServer::removeRegisterComponent(
	const std::string cid, 
	const int type /* = 0 */)
{
	int e{ !cid.empty() && 0 < type ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AbstractComponentPtr acp{ registerComponentGroup.at(cid) };
		registerComponentGroup.remove(cid);

		LOG(WARNING) << "Remove register component type = " << static_cast<int>(acp->getComponentType()) <<
			", ID = " << acp->getComponentID() <<
			", Name = " << acp->getComponentName() <<
			", Timestamp = " << acp->getComponentTimestamp() <<
			"[ Expire = " << Time().tickcount() - acp->getComponentTimestamp() << " ].";
	}

	return e;
}

int MDCServer::updateRegisterCompnent(void* c /* = nullptr */)
{
	int e{ c ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		//服务端找到匹配的ID标识则执行更新操作
		//如果没有匹配的ID标识则执行新增操作

		AbstractComponent* ac{ reinterpret_cast<AbstractComponent*>(c) };
		AbstractComponentPtr acp{ registerComponentGroup.at(ac->getComponentID()) };

		if (acp && !acp->getComponentID().compare(ac->getComponentID()) && acp->getComponentType() == ac->getComponentType())
		{
			acp->setComponentTimestamp(Time().tickcount());
			LOG(INFO) << "Update information of component, type = " << static_cast<int>(ac->getComponentType()) <<
				", ID = " << ac->getComponentID() << ".";
		}
		else
		{
			addRegisterComponent(c);
		}
	}

	return e;
}

int MDCServer::replyQueryRegisterComponent(
	const std::vector<AbstractComponentPtr>& componentPtrs,
	const int result /* = 0 */,
	const long long sequence /* = 0 */,
	void* s /* = nullptr */,
	const void* commID /* = nullptr */,
	const unsigned int idbytes /* = 0 */)
{
	int e{ s && 0 < sequence && commID && 0 < idbytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AbstractPacket* pkt{
			new(std::nothrow) MessagePacket(
				base::packet::PacketType::PACKET_TYPE_COMPONENT,
				static_cast<int>(base::protocol::ComponentCommand::COMPONENT_COMMAND_QUERY_REP),
				result) };

		if (pkt)
		{
			pkt->setPacketSequence(sequence);
			pkt->setPacketData((void*)&componentPtrs);
			const std::string rep{
				base::protocol::CommandPacker().packPacketToArray(pkt) };
			e = rep.empty() ? eBadPackProtocol : sendServerResponseMessage(s, commID, idbytes, rep.c_str(), rep.length());

			LOG(INFO) << "Query information of component count = " << componentPtrs.size() << ".";
		}
		else
		{
			e = eBadNewObject;
			LOG(WARNING) << "Bad create new object when querying information of component.";
		}
	}

	return e;
}

int MDCServer::replyConfigureSetFailedMessage(
	const int command /* = 0 */, 
	const long long sequence /* = 0 */, 
	void* s /* = nullptr */, 
	const void* commID /* = nullptr */, 
	const unsigned int idbytes /* = 0 */)
{
	int e{ s && 0 < sequence && commID && 0 < idbytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		int msgType{ 0 };
		void* data;

		if (static_cast<int>(StatusCommand::STATUS_COMMAND_SET_REP) == command)
		{
			data = base::protocol::StatusPacker().packStatus(command, eBadOperate);
			msgType = static_cast<int>(MessageType::MESSAGE_TYPE_STATUS);
		} 
		else if(static_cast<int>(AlgorithmCommand::ALGORITHM_COMMAND_SET_REP) == command)
		{
			data = base::protocol::AlgorithmPacker().packAlgorithm(command, eBadOperate);
			msgType = static_cast<int>(MessageType::MESSAGE_TYPE_ALGORITHM);
		}
		else if (static_cast<int>(CrewCommand::CREW_COMMAND_NEW_REP) == command ||
			static_cast<int>(CrewCommand::CREW_COMMAND_DELETE_REP) == command ||
			static_cast<int>(CrewCommand::CREW_COMMAND_MODIFY_REP) == command)
		{
			data = base::protocol::CrewPacker().packCrew(command, eBadOperate);
			msgType = static_cast<int>(MessageType::MESSAGE_TYPE_CREW);
		}

		if (data && 0 < msgType)
		{
			const std::string rep{
				base::protocol::MessagePacker().packMessage(msgType, sequence, data) };
			e = rep.empty() ? eBadPackProtocol : sendServerResponseMessage(s, commID, idbytes, rep.c_str(), rep.length());

			if (eSuccess == e)
			{
				LOG(INFO) << "Reply configuration set message successfully.";
			} 
			else
			{
				LOG(WARNING) << "Reply configuration set message failed, result = " << e << ".";
			}
		}
		else
		{
			e = eBadNewObject;
			LOG(WARNING) << "Reply configuration set message failed, result = " << e << ".";
		}
	}
	else
	{
		LOG(ERROR) << 
			"Can not reply failure configuration set message with invalid parameters, s = " << s << 
			", sequence = " << sequence << 
			", commID = " << reinterpret_cast<const char*>(commID) << ".";
	}

	return e;
}

int MDCServer::sendServerResponseMessage(
	void* s /* = nullptr */, 
	const void* commID /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	const void* data /* = nullptr */, 
	const unsigned int databytes /* = 0 */)
{
	int e{ s && commID && 0 < idbytes && data && 0 < databytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AsynchronousSender as;
		const unsigned int sentBytes{ 
			as.sendData(s, commID, idbytes, true) + as.sendData(s, data, databytes) };
		e = 0 < sentBytes ? eSuccess : eBadSend;
	}

	return e;
}

int MDCServer::forwardConfigureRequestOrResponseMessage(
	void* s /* = nullptr */, 
	const int type /* = 0 */, 
	const void* data /* = nullptr */, 
	const unsigned int databytes /* = 0 */)
{
	//先查找对应的组件再转发消息
	//每次查找组件必须遍历所有组件,并向满足条件的组件发送消息

	int e{ s && 0 < type && data && 0 < databytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		e = eBadOperate;
		std::vector<AbstractComponentPtr> components;
		registerComponentGroup.values(components);

		for (int i = 0; i != components.size(); ++i)
		{
			if (type == static_cast<int>(components[i]->getComponentType()))
			{
				const std::string delimiter{ " " }, commID{ components[i]->getComponentID() };
				AsynchronousSender as;
				const unsigned int sentBytes{
					as.sendData(s, commID.c_str(), commID.length(), true) +
					as.sendData(s, delimiter.c_str(), delimiter.length(), true) +
					as.sendData(s, data, databytes) };
				e = 0 < sentBytes ? eSuccess : eBadSend;
			}
		}
	}

	return e;
}
