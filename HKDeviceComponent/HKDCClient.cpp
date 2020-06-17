#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Error.h"
#include "Protocol/MessageDefs.h"
#include "HKDCClient.h"

HKDCClient::HKDCClient(const std::string cname)
	: NetworkAsynchronousClient(), clientType{ static_cast<int>(ComponentType::COMPONENT_TYPE_HKD) }, clientName{ cname }
{}
HKDCClient::~HKDCClient() {}

void HKDCClient::unpackMessageFailure()
{
	LOG(ERROR) << "Unpack message failed.";
}

void HKDCClient::parseUnknownMessage()
{
	LOG(WARNING) << "Parsed a unknown message and nothing to do.";
}

void HKDCClient::parseComponentMessage(void* pkt /* = nullptr */)
{
	MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
	ComponentResponse* cr{ reinterpret_cast<ComponentResponse*>(mp->packet) };
	clientID = cr->cid;
	NetworkAsynchronousClient::parseComponentMessage(pkt);

	LOG(INFO) << "Parsed message type = " << static_cast<int>(mp->type) << ", sequence = " << mp->sequence << ", timestamp = " << mp->timestamp <<
		", response result = " << cr->result << ".";
}

int HKDCClient::getClientType() const
{
	return clientType;
}

const char* HKDCClient::getClientID() const
{
	return clientID.c_str();
}

const char* HKDCClient::getClientName() const
{
	return clientName.c_str();
}
