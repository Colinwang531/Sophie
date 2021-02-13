//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					XMS会议室类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//

#ifndef XMS_CONFERENCE_H
#define XMS_CONFERENCE_H

#include "boost/asio.hpp"
#include "boost/shared_ptr.hpp"
#include "DataStruct/Vector.h"
#include "Network/ASIO/Session/TCPSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;

class XMSConference
{
public:
	XMSConference(void);
	~XMSConference(void);

public:
	int addSource(
		boost::asio::ip::tcp::socket& so, 
		const std::string url,
		const char* ipv4 = nullptr);
	int addTarget(TCPSessionPtr tsp);

private:
	void afterTCPSendResultCallback(
		const int e = 0,
		const int bytes = 0);
	void afterTCPReceiveResultCallback(
		const int e = 0,
		const void* data = nullptr,
		const int bytes = 0);

private:
	TCPSessionPtr source;
	Vector<TCPSessionPtr> targets;
};//class XMS

#endif//XMS_CONFERENCE_H
