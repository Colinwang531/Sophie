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
#include "Filter/Ai/CVFaceRecognizeDetectFilter.h"
using AbstractFilter = base::stream::AbstractFilter;
using CVFaceRecognizeDetectFilter = base::stream::CVFaceRecognizeDetectFilter;
using AbstractFilterPtr = boost::shared_ptr<AbstractFilter>;
using CVFaceRecognizeDetectFilterPtr = boost::shared_ptr<CVFaceRecognizeDetectFilter>;
#include "FaceRecognizeDetectLivestreamGraph.h"
#include "FaceRecognizeMediaStreamSession.h"

FaceRecognizeMediaStreamSession::FaceRecognizeMediaStreamSession(
	AbstractClient& parent,
	const std::string uid,
	const std::string url,
	const AbstractAlgorithm algo,
	boost::asio::ip::tcp::socket* s /* = nullptr */)
	: TCPStreamTargetSession(s), parentClient{ parent }, streamURL{ url }, algorithmParam{ algo }, frameSequence{ 0 }, uuid{ uid }
{}
FaceRecognizeMediaStreamSession::~FaceRecognizeMediaStreamSession() {}

int FaceRecognizeMediaStreamSession::startSession()
{
	int e{ createNewFaceRecognizeGraph() };

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

int FaceRecognizeMediaStreamSession::stopSession()
{
	int e{ TCPSession::stopSession() };

	if (eSuccess == e && livestreamFaceRecognizeGraphPtr)
	{
		e = livestreamFaceRecognizeGraphPtr->destroyGraph();
		livestreamFaceRecognizeGraphPtr.reset();
	}

	return e;
}

int FaceRecognizeMediaStreamSession::setFaceImage(
	const int id /* = -1 */, 
	const char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	int e{ -1 < id && data && 0 < bytes ? eSuccess : eInvalidParameter };

	if (eSuccess == e)
	{
		e = eBadOperate;

		if (livestreamFaceRecognizeGraphPtr)
		{
			AbstractFilterPtr filter{ 
				livestreamFaceRecognizeGraphPtr->getFilterByName(base::stream::CVAlgorithmFilterName) };
			if (filter)
			{
				CVFaceRecognizeDetectFilterPtr face{ 
					boost::dynamic_pointer_cast<CVFaceRecognizeDetectFilter>(filter) };
				e = face->addNewFaceImage(id, data, bytes);
			}
		}
	}

	return e;
}

void FaceRecognizeMediaStreamSession::receivedDataNotification(
	const unsigned char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	if (livestreamFaceRecognizeGraphPtr)
	{
		StreamPacketPtr pkt{
			boost::make_shared<StreamPacket>(
				base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_HK,
				base::packet::StreamFrameType::STREAM_FRAME_TYPE_NONE) };

		if (pkt)
		{
			pkt->setStreamData(data, bytes);
			livestreamFaceRecognizeGraphPtr->inputData(pkt);
		}
	}

	//数据处理完成之后才能再次接收
	//否则会导致数据乱序的问题
	//但是否可以考虑将数据缓存在每次接收时动态创建以解决这个问题
	TCPStreamTargetSession::receivedDataNotification(data, bytes);
}

int FaceRecognizeMediaStreamSession::createNewFaceRecognizeGraph()
{
	int e{ livestreamFaceRecognizeGraphPtr ? eObjectExisted : eSuccess };

	if (eSuccess == e)
	{
		AbstractGraphPtr graph{ 
			boost::make_shared<FaceRecognizeDetectLivestreamGraph>() };

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
							&FaceRecognizeMediaStreamSession::alarmDataNotificationCallback, this, _1));
				}

				livestreamFaceRecognizeGraphPtr.swap(graph);
			}
		}
	}

	return e;
}

int FaceRecognizeMediaStreamSession::sendURLFrameData(const std::string url)
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

void FaceRecognizeMediaStreamSession::alarmDataNotificationCallback(StreamPacketPtr pkt)
{
	if (pkt && !alarmComponentID.empty())
	{
		boost::shared_ptr<DataPacket> datapkt{
			boost::make_shared<MessagePacket>(
				base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALARM) };

		if (datapkt)
		{
			int similar{ 0 }, algorithmType{ 0 }, matchID{ 0 }, h{ 0 };
			pkt->getAlarmRange(similar, algorithmType, matchID, h);

			std::string msg;
 			MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(datapkt) };
			msgpkt->setMessagePacketCommand(static_cast<int>(algorithmType));
			datapkt->setPacketData((void*)pkt->getStreamData());
			datapkt->setPacketData((void*)streamURL.c_str());
			//设置一个无效的母钟时间数据便于打包时使用
			datapkt->setPacketData((void*)"2020-10-10 19:00:00");
			datapkt->setPacketData((void*)&similar);
			datapkt->setPacketData((void*)&algorithmType);
			datapkt->setPacketData((void*)&matchID);
			datapkt->setPacketData((void*)&h);
			//设置一个无效的用户ID
			char uid[32]{ 0 };
#ifdef WINDOWS
			memcpy_s(uid, 32, &matchID, 32);
#else
			memcpy(uid, &matchID, 32);
#endif//WINDOWS
			datapkt->setPacketData(uid);
			datapkt->setPacketSequence(1);
			datapkt->setPacketTimestamp(Time().tickcount());
			msg = DataPacker().packData(datapkt);

			parentClient.sendData("worker", "notification", uuid, alarmComponentID, msg);
			LOG(INFO) << "Face recognize alarm match ID = " << matchID << ", similar = " << similar << ", type = " << algorithmType << ".";
		}
	}
}
