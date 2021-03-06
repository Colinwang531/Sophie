//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					安全帽算法流会话类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//

#include "boost/shared_ptr.hpp"
#include "Graph/AbstractGraph.h"
using AbstractGraph = base::stream::AbstractGraph;
using AbstractGraphPtr = boost::shared_ptr<AbstractGraph>;
#include "AI/AbstractAlgorithm.h"
using AbstractAlgorithm = base::ai::AbstractAlgorithm;
#include "Network/AbstractClient.h"
using AbstractClient = base::network::AbstractClient;
#include "Session/TCPStreamTargetSession.h"
using TCPStreamTargetSession = base::network::TCPStreamTargetSession;

class HelmetMediaStreamSession : public TCPStreamTargetSession
{
public:
	HelmetMediaStreamSession(
		AbstractClient& parent,
		const std::string uid,
		const std::string url,
		const AbstractAlgorithm algo,
		boost::asio::ip::tcp::socket* s = nullptr);
	~HelmetMediaStreamSession(void);

public:
	int startSession(void) override;
	int stopSession(void) override;
	inline void setAlarmComponentID(const std::string id)
	{
		alarmComponentID = id;
	}
	
protected:
	void receivedDataNotification(
		const unsigned char* data = nullptr, 
		const int bytes = 0) override;

private:
	int createNewHelmetGraph(void);
	int sendURLFrameData(const std::string url);
	void alarmDataNotificationCallback(StreamPacketPtr pkt);

private:
	AbstractClient& parentClient;
	const std::string uuid;
	const std::string streamURL;
	std::string alarmComponentID;
	const AbstractAlgorithm algorithmParam;
	long long frameSequence;
	AbstractGraphPtr livestreamHelmetGraphPtr;
};//class HelmetMediaStreamSession

