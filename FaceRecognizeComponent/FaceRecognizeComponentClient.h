//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					人脸识别法组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-06-15									创建
//

#include "boost/shared_ptr.hpp"
#include "Packet/DataPacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
#include "Device/Hikvision/HikvisionDevice.h"
using AbstractDevice = base::device::AbstractDevice;
using SurveillanceDevice = base::device::SurveillanceDevice;
using HikvisionDevice = base::device::HikvisionDevice;
using AbstractDevicePtr = boost::shared_ptr<base::device::AbstractDevice>;
#include "DataStruct/UnorderedMap.h"
using AbstractDeviceGroup = UnorderedMap<const std::string, AbstractDevicePtr>;
#include "FaceRecognizeMediaStreamSession.h"
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
using FaceRecognizeMediaStreamSessionPtr = boost::shared_ptr<FaceRecognizeMediaStreamSession>;
using FaceRecognizeStreamSessionGroup = UnorderedMap<const std::string, TCPSessionPtr>;
#include "Network/AbstractMediaStreamClient.h"
using AbstractMediaStreamClient = base::network::AbstractMediaStreamClient;

class FaceRecognizeComponentClient : public AbstractMediaStreamClient
{
public:
	FaceRecognizeComponentClient(
		const std::string address,
		const unsigned short port = 60531);
	virtual ~FaceRecognizeComponentClient(void);

protected:
	void afterClientPolledMessageProcess(
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg) override;
	const std::string buildAutoRegisterToBrokerMessage(void) override;
	const std::string buildAutoQueryRegisterSubroutineMessage(void) override;
	int createNewMediaStreamSession(
		const std::string url,
		boost::asio::ip::tcp::socket* s) override;

private:
	const std::string getAlgorithmClientInfoByName(const std::string name) const;
	void setAlgorithmClientInfoWithName(
		const std::string name, 
		const std::string value);
	void processComponentMessage(
		const std::string fromID,
		DataPacketPtr pkt);
	void processAlgorithmMessage(
		const std::string fromID,
		DataPacketPtr pkt);
	void processStatusMessage(
		const std::string fromID,
		DataPacketPtr pkt);
	void processCrewMessage(
		const std::string fromID,
		DataPacketPtr pkt);
	int replyMessageWithResult(
		const std::string fromID,
		const int type = 0,
		const int command = 0,
		const int result = 0,
		const long long sequence = 0);

private:
	int sailStatus;
	FaceRecognizeStreamSessionGroup streamSessionGroup;
	Vector<AbstractAlgorithm> algorithmParamGroup;
};//class FaceRecognizeComponentClient

