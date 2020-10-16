//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					报警推送组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-10-03									创建
//

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
#include "Network/AbstractClient.h"
using AbstractClient = base::network::AbstractClient;

class AlarmPusherComponentClient : public AbstractClient
{
public:
	AlarmPusherComponentClient(void);
	virtual ~AlarmPusherComponentClient(void);

protected:
	void afterClientPolledMessageProcess(
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg) override;
	const std::string buildAutoRegisterToBrokerMessage(void) override;
	const std::string buildAutoQueryRegisterSubroutineMessage(void) override;

private:
	const std::string getMediaStreamClientInfoByName(const std::string name) const;
	void setMediaStreamClientInfoWithName(
		const std::string name, 
		const std::string value);
	void processComponentMessage(DataPacketPtr pkt);
	void processAlarmMessage(const std::string msg);

private:
	std::string webComponentID;
};//class AlarmPusherComponentClient

