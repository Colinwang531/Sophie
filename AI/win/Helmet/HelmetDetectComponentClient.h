//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					��ȫñ�㷨����ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
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
#include "DataStruct/Vector.h"
#include "DataStruct/UnorderedMap.h"
using AbstractDeviceGroup = UnorderedMap<const std::string, AbstractDevicePtr>;
#include "HelmetMediaStreamSession.h"
using TCPSessionPtr = boost::shared_ptr<TCPSession>;
using HelmetStreamSessionPtr = boost::shared_ptr<HelmetMediaStreamSession>;
using HelmetStreamSessionGroup = UnorderedMap<const std::string, TCPSessionPtr>;
#include "ASIO/ASIOService.h"
using ASIOService = base::network::ASIOService;
#include "MessageQueue/MajordomoWorker.h"
using MajordomoWorker = mq::module::MajordomoWorker;
using MajordomoWorkerPtr = boost::shared_ptr<MajordomoWorker>;
#include "Network/AbstractWorker.h"
using AbstractWorker = base::network::AbstractWorker;

class HelmetDetectComponentClient : public AbstractWorker
{
public:
	HelmetDetectComponentClient(
		const std::string address,
		const unsigned short port = 60531);
	virtual ~HelmetDetectComponentClient(void);

public:
	int sendData(
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data) override;

protected:
	int createNewClient(
		const std::string address) override;
	int destroyClient(void) override;
	//WORKER�����ݶ�ȡ�ص�
	//@roleID : ��ɫID��ʶ
	//@flagID : ��־ID��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@data : ��Ϣ����
	//@Return : ������
	void afterPolledDataFromWorkerCallback(
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data);
	void sendRegisterWorkerServerMessage(void) override;
	void sendQuerySystemServiceMessage(void) override;
	int createNewMediaStreamSession(
		const std::string url,
		boost::asio::ip::tcp::socket* s);

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
	int replyMessageWithResult(
		const std::string fromID,
		const std::string toID,
		const int type = 0,
		const int command = 0,
		const int result = 0,
		const long long sequence = 0);

	//���ӷ����
	//@did : �豸ID��ʶ
	//@cid : �����ID��ʶ
	//@idx : ���������
	//@Return : ������
	int connectMediaServer(
		const std::string did,
		const std::string cid,
		const int idx = -1);

	//�Ͽ����������
	//@did : �豸ID��ʶ
	//@Return : ������
	//@Comment : �Ͽ��豸�����е�����
	int disconnectMediaServer(const std::string did);

	void afterRemoteConnectedNotificationCallback(
		boost::asio::ip::tcp::socket* s,
		const boost::system::error_code e);

private:
	ASIOService asioService;
	const std::string mediaIP;
	const unsigned short mediaPort;
	int sailStatus;
	HelmetStreamSessionGroup streamSessionGroup;
	Vector<AbstractAlgorithm> algorithmParamGroup;
	Vector<std::string> urlGroup;
	MajordomoWorkerPtr worker;
	std::string parentXMQID;
};//class HelmetDetectComponentClient

