#include "boost/bind.hpp"
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
#include "Protocol/MessageDefs.h"
#include "Protocol/Alarm/AlarmCodec.h"
#include "Protocol/Algorithm/AlgorithmCodec.h"
#include "Protocol/Component/ComponentCodec.h"
using ComponentParser = base::protocol::ComponentParser;
#include "Protocol/Crew/CrewCodec.h"
#include "Protocol/Device/DeviceCodec.h"
#include "Protocol/Status/StatusCodec.h"
#include "Protocol/MessageCodec.h"
using MessageParser = base::protocol::MessageParser;
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
				boost::bind(&MDCServer::updateComponentCheckingThreadProc, this)) };
		e = t ? eSuccess : eBadNewThread;
	}

	return e;
}

void MDCServer::afterServerPollMessage(
	void* s /* = nullptr */, 
	const char* id /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	const char* delimiter /* = nullptr */, 
	const unsigned int delimiterbytes /* = 0 */, 
	const char* data /* = nullptr */, 
	const unsigned int databytes /* = 0 */)
{
	//Just for testing.
// 	LOG(INFO) << "[ ID:" << idbytes << " ] " << id;
// 	LOG(INFO) << "[ Delimiter:" << delimiterbytes << " ] " << delimiter;
// 	LOG(INFO) << "[ Data:" << databytes << " ] " << data;

	//解析MSG封装数据获取消息类型和内容
	MessageParser parser;
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(parser.createNewPacket()) };
	
	if (eSuccess == parser.unpackMessage(data, mp))
	{
		switch (mp->type)
		{
			case MessageType::MESSAGE_TYPE_ALARM:
			{
				break;
			}
			case MessageType::MESSAGE_TYPE_ALGORITHM:
			{
				break;
			}
			case MessageType::MESSAGE_TYPE_COMPONENT:
			{
				dealWithComponentMessage(s, id, idbytes, mp);
				break;
			}
			case MessageType::MESSAGE_TYPE_CREW:
			{
				break;
			}
			case MessageType::MESSAGE_TYPE_DEVICE:
			{
				break;
			}
			case MessageType::MESSAGE_TYPE_STATUS:
			{
				dealWithStatusMessage(s, id, idbytes, mp);
				break;
			}
			case MessageType::MESSAGE_TYPE_USER:
			{
				break;
			}
			default:
			{
				LOG(INFO) << "Recieved unknown message.";
				break;
			}
		}
	}
	else
	{
		LOG(WARNING) << "Can not parse message.";
	}

	//销毁消息包实例
	parser.destroyPacket(mp);
}

void MDCServer::afterUpstreamPollMessage(
	const char* msg /* = nullptr */, 
	const unsigned int bytes /* = 0 */)
{
}

void MDCServer::updateComponentCheckingThreadProc()
{
	long long startTime{ Time().tickcount() };

	while (!stopped)
	{
		long long nowTime{ Time().tickcount() };

		//每30秒检测一次
		if (30000 < nowTime - startTime)
		{
			startTime = nowTime;

			std::vector<void*> rcis;
			registerComponentInfos.values(rcis);
			for (int i = 0; i != rcis.size(); ++i)
			{
				ComponentInfo* info{ reinterpret_cast<ComponentInfo*>(rcis[i]) };

				//超时90秒移除组件信息
				if (90000 < nowTime - info->timestamp)
				{
					removeComponentInfo(info->cid, static_cast<int>(info->type));
				}
			}
		}

		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}

void MDCServer::dealWithComponentMessage(
	void* s /* = nullptr */, 
	const char* mid /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	void* pkt /* = nullptr */)
{
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
	ComponentPacket* cp{ reinterpret_cast<ComponentPacket*>(mp->packet) };
	const ComponentCommand command{ reinterpret_cast<ComponentCommand>(cp->command) };

	if (ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == command)
	{
		ComponentRequest* req{ reinterpret_cast<ComponentRequest*>(cp->data) };

		//cid空表示组件请求注册
		//cid不空标识组件请求心跳更新
		if (req->cinfo.cid.empty())
		{
			const std::string cid{ 
				addComponentInfo(req->cinfo.cname, static_cast<int>(req->cinfo.type)) };
			replyAddComponentInfo(
				cid, cid.empty() ? eBadOperate : eSuccess, mp->sequence + 1, s, mid, idbytes);
		}
		else
		{
			updateCompnentInfo(req->cinfo.cid, static_cast<int>(req->cinfo.type));
		}
	}
	else if (ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == command)
	{
		ComponentRequest* req{ reinterpret_cast<ComponentRequest*>(cp->data) };
		removeComponentInfo(req->cinfo.cid, static_cast<int>(req->cinfo.type));
	}
	else if (ComponentCommand::COMPONENT_COMMAND_QUERY_REQ == command)
	{
		std::vector<void*> rcis;
		registerComponentInfos.values(rcis);
		replyQueryComponentInfo(rcis, eSuccess, mp->sequence + 1, s, mid, idbytes);
	}
}

void MDCServer::dealWithStatusMessage(
	void* s /* = nullptr */, 
	const char* mid /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	void* pkt /* = nullptr */)
{
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
	StatusPacket* sp{ reinterpret_cast<StatusPacket*>(mp->packet) };
	const StatusCommand command{ reinterpret_cast<StatusCommand>(sp->command) };

	if (StatusCommand::STATUS_COMMAND_SET_REQ == command)
	{
		StatusRequest* req{ reinterpret_cast<StatusRequest*>(sp->data) };

		switch (req->type)
		{
			case StatusType::STATUS_TYPE_SAIL:
			{
				sailStatus = req->flag;
				break;
			}
			case StatusType::STATUS_TYPE_AUTO:
			{
				autoStatus = req->flag;
				break;
			}
			case StatusType::STATUS_TYPE_NAME:
			{
				nodeName = req->name;
				break;
			}
			default:
				break;
		}
	}
	else if (StatusCommand::STATUS_COMMAND_QUERY_REQ == command)
	{	
	}
}

const std::string MDCServer::addComponentInfo(
	const std::string cname, const int type /* = 0 */)
{
	std::string uuidStr{ boost::uuids::to_string(boost::uuids::random_generator()()) };

	if (0 < type && !uuidStr.empty())
	{
		ComponentInfo* info{ new(std::nothrow) ComponentInfo };

		if (info)
		{
			info->type = static_cast<ComponentType>(type);
			info->cid = uuidStr;
			info->cname = cname;
			info->timestamp = Time().tickcount();
			registerComponentInfos.insert(uuidStr, info);

			LOG(INFO) << "Add register component type = " << type << 
				", ID = " << uuidStr << 
				", Name = " << cname << 
				", Timestamp = " << info->timestamp << ".";
		} 
		else
		{
			uuidStr.clear();
		}
	}

	return uuidStr;
}

int MDCServer::replyAddComponentInfo(
	const std::string cid, 
	const int result /* = 0 */, 
	const long long sequence /* = 0 */, 
	void* s /* = nullptr */, 
	const char* mid /* = nullptr */, 
	const unsigned int idbytes /* = 0 */)
{
	int e{ s && 0 < sequence && mid && 0 < idbytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		base::protocol::ComponentPacker cp;
		void* c{ 
			cp.packComponent(
				static_cast<int>(ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP), result, (void*)cid.c_str()) };

		if (c)
		{
			const std::string rep{
				base::protocol::MessagePacker().packMessage(static_cast<int>(MessageType::MESSAGE_TYPE_COMPONENT), sequence, c) };
			e = rep.empty() ? eBadPackProtocol : sendReplyMessage(s, mid, idbytes, rep.c_str(), rep.length());
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

int MDCServer::removeComponentInfo(
	const std::string cid, 
	const int type /* = 0 */)
{
	int e{ !cid.empty() && 0 < type ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		ComponentInfo* info{ reinterpret_cast<ComponentInfo*>(registerComponentInfos.at(cid)) };
		registerComponentInfos.remove(cid);

		LOG(WARNING) << "Remove register component type = " << static_cast<int>(info->type) <<
			", ID = " << info->cid <<
			", Name = " << info->cname <<
			", Timestamp = " << info->timestamp << 
			"[ Expire = " << Time().tickcount() - info->timestamp << " ].";

		boost::checked_delete(info);
	}

	return e;
}

int MDCServer::updateCompnentInfo(
	const std::string cid, 
	const int type /* = 0 */)
{
	int e{ !cid.empty() && 0 < type ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		ComponentInfo* info{ reinterpret_cast<ComponentInfo*>(registerComponentInfos.at(cid)) };
		if (!info->cid.compare(cid) && info->type == static_cast<ComponentType>(type))
		{
			info->timestamp = Time().tickcount();
			LOG(INFO) << "Update timestamp of component type = " << type << ", ID = " << cid << ".";
		}
		else
		{
			e = eInvalidParameter;
		}
	}

	return e;
}

int MDCServer::replyQueryComponentInfo(
	const std::vector<void*>& rcis,
	const int result /* = 0 */,
	const long long sequence /* = 0 */,
	void* s /* = nullptr */,
	const char* mid /* = nullptr */,
	const unsigned int idbytes /* = 0 */)
{
	int e{ s && 0 < sequence && mid && 0 < idbytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		base::protocol::ComponentPacker cp;
		void* c{
			cp.packComponent(
				static_cast<int>(ComponentCommand::COMPONENT_COMMAND_QUERY_REP), result, (void*)&rcis) };

		if (c)
		{
			const std::string rep{
				base::protocol::MessagePacker().packMessage(static_cast<int>(MessageType::MESSAGE_TYPE_COMPONENT), sequence, c) };
			e = rep.empty() ? eBadPackProtocol : sendReplyMessage(s, mid, idbytes, rep.c_str(), rep.length());
			LOG(INFO) << "Query component informations result = " << e << ".";
		}
		else
		{
			e = eBadNewObject;
			LOG(WARNING) << "Query component informations result = " << e << ".";
		}
	}

	return e;
}

int MDCServer::sendReplyMessage(
	void* s /* = nullptr */, 
	const char* mid /* = nullptr */, 
	const unsigned int idbytes /* = 0 */, 
	const char* msg /* = nullptr */, 
	const unsigned int bytes /* = 0 */)
{
	int e{ s && mid && 0 < idbytes && msg && 0 < bytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		AsynchronousSender as;
		unsigned int sentBytes{ as.sendData(s, mid, idbytes, true) };

		if (0 < sentBytes)
		{
			sentBytes = as.sendData(s, msg, bytes);
		}

		e = 0 < sentBytes ? eSuccess : eBadSend;
	}

	return e;
}
