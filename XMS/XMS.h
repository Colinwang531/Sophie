//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					��ý�������
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//										������									2020-12-12									�ع���ý�����������
//

#ifndef XMS_H
#define XMS_H

#include "boost/shared_ptr.hpp"
#include "DataStruct/UnorderedMap.h"
#include "Network/ASIO/Session/TCPSession.h"
using TCPSession = framework::network::asio::TCPSession;
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
#include "Network/Pattern/XMQPeer.h"
using XMQPeer = framework::network::pattern::XMQPeer;
#include "XMSConference.h"
using XMSConferencePtr = boost::shared_ptr<XMSConference>;
using XMSConferenceGroup = UnorderedMap<std::string, XMSConferencePtr>;

class XMS final : public XMQPeer
{
public:
	XMS(
		const std::string name,
		const std::string id,
		const unsigned short port = 0);
	~XMS(void);

public:
	int startOD(
		const char* ipv4 = nullptr,
		const unsigned short port = 0) override;
	int stopOD(void) override;

protected:
	void afterParsePolledMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::vector<std::string> routers,
		const std::vector<std::string> messages) override;
	const std::string buildRegisterMessage(void) override;
	void processComponentMessage(
		const std::string module,
		const std::string from,
		const std::string to,
		const std::string sequence,
		const std::string timestamp,
		const std::string msg);

private:
	void afterTCPAcceptResultCallback(
		const int e,
		boost::asio::ip::tcp::socket& so);
	void afterTCPTargetReceiveResultCallback(
		const char* data = nullptr, 
		const int bytes = 0,
		TCPSessionPtr tsp = nullptr);

private:
	const unsigned short mediaPort;
	XMSConferenceGroup conferences;
};//class XMS

#endif//XMS_H
