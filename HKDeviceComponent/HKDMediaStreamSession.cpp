#include "boost/algorithm/string.hpp"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
#include "Error.h"
#include "Time/XTime.h"
using Time = base::time::Time;
#include "Packet/Stream/StreamPacket.h"
using DataPacketPtr = boost::shared_ptr<base::packet::DataPacket>;
using StreamPacket = base::packet::StreamPacket;
using StreamPacketPtr = boost::shared_ptr<StreamPacket>;
#include "HKDMediaStreamSession.h"

HKDMediaStreamSession::HKDMediaStreamSession(
	const std::string url,
	const int uid /* = -1 */,
	boost::asio::ip::tcp::socket* s /* = nullptr */)
	: TCPStreamTargetSession(s), streamURL{ url }, userID{ uid },
	streamID{ -1 }, frameSequence{ 0 }, frameSysHead{ nullptr }, frameSysHeadBytes{ 0 }
{}
HKDMediaStreamSession::~HKDMediaStreamSession() {}

int HKDMediaStreamSession::startSession()
{
	int e{ TCPSession::startSession() };

	if (eSuccess == e)
	{
		e = sendURLFrameData(streamURL);
		if (eSuccess == e)
		{
			std::vector<std::string> idGroup;
			boost::split(idGroup, streamURL, boost::is_any_of(":"));

			NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
			struPlayInfo.lChannel = atoi(idGroup[idGroup.size() - 1].c_str()); //Ԥ��ͨ����
			struPlayInfo.dwStreamType = 0; //0-��������1-��������2-����3��3-����4���Դ�����
			struPlayInfo.dwLinkMode = 0; //0- TCP ��ʽ��1- UDP ��ʽ��2- �ಥ��ʽ��3- RTP ��ʽ��4-RTP/RTSP��5-RSTP/HTTP
			streamID = NET_DVR_RealPlay_V40(
				userID,
				&struPlayInfo,
				&HKDMediaStreamSession::afterGotRealplayDataNotificationCallback,
				this);
			e = 0 > streamID ? eBadOperate : eSuccess;
		}
	}

	return e;
}

int HKDMediaStreamSession::stopSession()
{
	int e{ 0 > streamID ? eBadOperate : eSuccess };

	if (eSuccess == e)
	{
		NET_DVR_StopRealPlay(streamID);
		boost::checked_array_delete(frameSysHead);
		e = TCPSession::stopSession();
	}

	return e;
}

void HKDMediaStreamSession::receivedDataNotification(
	const unsigned char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{}

int HKDMediaStreamSession::sendURLFrameData(const std::string url)
{
	int e{ url.empty() ? eInvalidParameter : eSuccess };

	if (eSuccess == e)
	{
		StreamPacket* pkt{
			new(std::nothrow) StreamPacket(
				base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_HK,
				base::packet::StreamFrameType::STREAM_FRAME_TYPE_STREAM_URL) };
		if (pkt)
		{
			//URL����ʱ����̶�Ϊ0
			pkt->setPacketSequence(0);
			pkt->setPacketTimestamp(Time().tickcount());

			if (eSuccess == pkt->buildStreamData((const unsigned char*)url.c_str(), url.length()))
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

void HKDMediaStreamSession::afterGotRealplayDataNotificationCallback(
	LONG sid /* = -1 */,
	DWORD type /* = 0 */,
	BYTE* data /* = nullptr */,
	DWORD bytes /* = 0 */,
	void* ctx /* = nullptr */)
{
	HKDMediaStreamSession* session{ reinterpret_cast<HKDMediaStreamSession*>(ctx) };

	//����ͷ��Ϣ
	if (NET_DVR_SYSHEAD == type)
	{
		boost::checked_array_delete(session->frameSysHead);
		session->frameSysHead = new(std::nothrow) BYTE[bytes];
		if (session->frameSysHead)
		{
#ifdef WINDOWS
			memcpy_s(session->frameSysHead, bytes, data, bytes);
#else
			memcpy(session->frameSysHead, data, bytes);
#endif
			session->frameSysHeadBytes = bytes;
		}
	}
	else
	{
		if (session && session->streamID == sid)
		{
			//���25֡����һ��ͷ��Ϣ
			if (0 == (session->frameSequence % 25) && session->frameSysHead && 0 < session->frameSysHeadBytes)
			{
				StreamPacket* pkt{ 
					new(std::nothrow) StreamPacket(
						base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_HK,
						base::packet::StreamFrameType::STREAM_FRAME_TYPE_HEADER) };
				if (pkt)
				{
					//ͷ��Ϣ����ʱ����̶�Ϊ0
					pkt->setPacketSequence(0);
					pkt->setPacketTimestamp(Time().tickcount());
					if (eSuccess == pkt->buildStreamData(session->frameSysHead, session->frameSysHeadBytes))
					{
						session->sendData(pkt);
					}
				}
			}

			StreamPacket* pkt{
					new(std::nothrow) StreamPacket(
						base::packet::StreamPacketType::STREAM_PACKET_TYPE_PRIVATE_HK,
						base::packet::StreamFrameType::STREAM_FRAME_TYPE_STREAM_DATA) };
			if (pkt)
			{
				pkt->setPacketSequence(++session->frameSequence);
				pkt->setPacketTimestamp(Time().tickcount());
				if (eSuccess == pkt->buildStreamData(data, bytes))
				{
					session->sendData(pkt);
				}
			}
		}
	}
}
