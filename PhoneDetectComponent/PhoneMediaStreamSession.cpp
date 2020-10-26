#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
#ifdef WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//WINDOWS
#include "Error.h"
#include "Time/XTime.h"
using Time = base::time::Time;
#include "Packet/Message/MessagePacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#include "Protocol/DataPhrase.h"
using DataPacker = base::protocol::DataPacker;
#include "Protocol/AlarmPhrase.h"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
#include "Filter/AbstractFilter.h"
using AbstractFilter = base::stream::AbstractFilter;
using AbstractFilterPtr = boost::shared_ptr<AbstractFilter>;
#include "PhoneDetectLivestreamGraph.h"
#include "PhoneMediaStreamSession.h"

PhoneMediaStreamSession::PhoneMediaStreamSession(
	AbstractClient& parent,
	const std::string uid,
	const std::string url, 
	const AbstractAlgorithm algo,
	boost::asio::ip::tcp::socket* s /* = nullptr */)
	: TCPStreamTargetSession(s), parentClient{ parent }, streamURL{ url }, algorithmParam{ algo }, frameSequence{ 0 }, uuid{ uid }
{}
PhoneMediaStreamSession::~PhoneMediaStreamSession() {}

int PhoneMediaStreamSession::startSession()
{
	int e{ createNewPhoneGraph() };

	if (eSuccess == e)
	{
		e = TCPSession::startSession();

		if (eSuccess == e)
		{
			e = sendURLFrameData(streamURL);
		}
	}

	return e;
}

int PhoneMediaStreamSession::stopSession()
{
	int e{ TCPSession::stopSession() };

	if (eSuccess == e && livestreamPhoneGraphPtr)
	{
		e = livestreamPhoneGraphPtr->destroyGraph();
		livestreamPhoneGraphPtr.reset();
	}

	return e;
}

void PhoneMediaStreamSession::receivedDataNotification(
	const unsigned char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	if (livestreamPhoneGraphPtr)
	{
		StreamPacketPtr pkt{
			boost::make_shared<StreamPacket>(
				base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_HK,
				base::packet::StreamFrameType::STREAM_FRAME_TYPE_NONE) };

		if (pkt)
		{
			pkt->setStreamData(data, bytes);
			livestreamPhoneGraphPtr->inputData(pkt);
		}
	}

	//数据处理完成之后才能再次接收
	//否则会导致数据乱序的问题
	//但是否可以考虑将数据缓存在每次接收时动态创建以解决这个问题
	TCPStreamTargetSession::receivedDataNotification(data, bytes);
}

int PhoneMediaStreamSession::createNewPhoneGraph()
{
	int e{ livestreamPhoneGraphPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		AbstractGraphPtr graph{ 
			boost::make_shared<PhoneDetectLivestreamGraph>() };

		if (graph)
		{
			e = graph->createNewGraph((void*)&algorithmParam);

			if (eSuccess == e)
			{
				AbstractFilterPtr filter{ 
					graph->getFilterByName(base::stream::JPEGImageEncoderFilterName) };
				if (filter)
				{
					filter->setDataCallback(
						boost::bind(
							&PhoneMediaStreamSession::alarmDataNotificationCallback, this, _1));
				}

				livestreamPhoneGraphPtr.swap(graph);
			}
		}
	}

	return e;
}

int PhoneMediaStreamSession::sendURLFrameData(const std::string url)
{
	int e{ url.empty() ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		StreamPacket* pkt{
			new(std::nothrow) StreamPacket(
				base::packet::StreamPacketType::STREAM_PACKET_TYPE_NONE,
				base::packet::StreamFrameType::STREAM_FRAME_TYPE_STREAM_URL) };
		if (pkt)
		{
			pkt->setPacketSequence(++frameSequence);
			pkt->setPacketTimestamp(Time().tickcount());
			if (eSuccess == pkt->buildStreamData((const unsigned char*)url.c_str(), (int)url.length()))
			{
				e = TCPSession::sendData(pkt);
			}
		}
		else
		{
			e = eBadNewObject;
		}
	}

	return e;
}

void PhoneMediaStreamSession::alarmDataNotificationCallback(StreamPacketPtr pkt)
{
	if (pkt && !alarmComponentID.empty())
	{
		boost::shared_ptr<DataPacket> datapkt{
			boost::make_shared<MessagePacket>(
				base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALARM) };

		if (datapkt)
		{
			int x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 };
			pkt->getAlarmRange(x, y, w, h);

			std::string msg;
 			MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(datapkt) };
			msgpkt->setMessagePacketCommand(
				static_cast<int>(base::protocol::AlarmType::ALARM_TYPE_PHONE));
			datapkt->setPacketData((void*)pkt->getStreamData());
			datapkt->setPacketData((void*)streamURL.c_str());
			//设置一个无效的母钟时间数据便于打包时使用
			datapkt->setPacketData(nullptr);
			datapkt->setPacketData((void*)&x);
			datapkt->setPacketData((void*)&y);
			datapkt->setPacketData((void*)&w);
			datapkt->setPacketData((void*)&h);
			//设置一个无效的用户ID
			datapkt->setPacketData(nullptr);
			datapkt->setPacketSequence(1);
			datapkt->setPacketTimestamp(Time().tickcount());
			msg = DataPacker().packData(datapkt);

			parentClient.sendData("worker", "notification", uuid, alarmComponentID, msg);
			LOG(INFO) << "Phone alarm x = " << x << ", y = " << y << ", w = " << w << ", h = " << h << ".";
		}
	}
}
