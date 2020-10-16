//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-13
//		Description :					��Ϣ�ַ��첽�������
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//

#include "boost/shared_ptr.hpp"
#include "DataStruct/UnorderedMap.h"
#include "Network/AbstractUpstreamServer.h"
using AbstractUpstreamServer = base::network::AbstractUpstreamServer;
using ServerModuleType = base::network::ServerModuleType;
using ClientModuleType = base::network::ClientModuleType;
#include "Packet/DataPacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
using AbstractComponentPtr = boost::shared_ptr<AbstractComponent>;

class MDCServer final : public AbstractUpstreamServer
{
public:
	MDCServer(
		const ServerModuleType server = ServerModuleType::SERVER_MODULE_TYPE_NONE,
		const ClientModuleType upstream = ClientModuleType::CLIENT_MODULE_TYPE_NONE,
		const std::string address = "tcp:\\127.0.0.1:61001",
		const std::string name = "MDCServer");
	~MDCServer(void);

protected:
	void afterServerPolledMessageProcess(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg) override;
	void afterClientPolledMessageProcess(
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg) override;
	void afterAutoCheckConnectionTimeoutProcess(void) override;
	const std::string buildAutoRegisterToBrokerMessage(void) override;

private:
	const std::string getMDCServerInfoByName(
		const std::string name) const;
	void setMDCServerInfoWithName(
		const std::string name, const std::string value);

	//��������ݽ��մ���
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@msg : ��Ϣ����
	void processServerPolledMessage(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg);
	//���������ת������
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@msg : ��Ϣ����
	void forwardServerPolledMessage(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg);

	//���������Ϣ
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@pkt : ��Ϣ����
	void processComponentMessage(
		const std::string commID, 
		const std::string flagID,
		const std::string fromID,
		DataPacketPtr pkt);

	//�����ע������
	//@componentID : ���ID��ʶ
	//@serviceName : �����������
	//@communicationID : ͨ��ID��ʶ
	//@componentType : �������
	//@Return : ������
	//@Comment : ͨ��replyAddNewRegisterComponent����Ӧ��
	int addNewRegisterComponent(
		const std::string componentID,
		const std::string serviceName,
		const std::string communicationID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

	//���������Ϣ
	//@componentID : ���ID��ʶ
	//@serviceName : �������
	//@communicationID : ͨ��ID��ʶ
	//@componentType : �������
	//@Return : ������
	//@Comment : ��Ӧ��
	int updateRegisterCompnent(
		const std::string componentID,
		const std::string serviceName,
		const std::string communicationID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

	//ɾ�����
	//@componentID : ���ID��ʶ
	//@Return : ������
	int removeComponentByID(const std::string componentID);

	//�����Ӧ����������Ϣ
	//@cid : ���ID��ʶ
	//@result : ע��״̬
	//@sequence : ��Ϣ���к�
	//@s : SOCKET��ʶ
	//@commID : ͨ��ID��ʶ
	//@idbytes : ID��ʶ�ֽ���
	//@Return : ������
	int replyMessageWithResultAndExtendID(
		const std::string commID,
		const std::string extendID,
		const std::string fromID,
		const int pkttype = 0,
		const int command = 0,
		const int result = 0,
		const long long sequence = 0);

private:
	//ע���������
	//KEY�����ID��ʶ
	using RegisterComponentGroup = UnorderedMap<const std::string, AbstractComponentPtr>;
	RegisterComponentGroup registerComponentGroup;
};//class MDCServer

