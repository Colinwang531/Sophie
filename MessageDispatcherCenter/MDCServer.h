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
#include "Network/MajordomoUpstreamBroker.h"
using MajordomoUpstreamBroker = base::network::MajordomoUpstreamBroker;
#include "Packet/DataPacket.h"
using DataPacket = base::packet::DataPacket;
using DataPacketPtr = boost::shared_ptr<DataPacket>;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
using AbstractComponentPtr = boost::shared_ptr<AbstractComponent>;

class MDCServer final : public MajordomoUpstreamBroker
{
public:
	MDCServer(void);
	~MDCServer(void);

protected:
	void afterPolledDataFromServerCallback(
		const std::string commID,
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data) override;
	void afterPolledDataFromWorkerCallback(
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data) override;
	void afterAutoClientConnectionTimeoutProcess(void) override;
	void sendRegisterWorkerServerMessage(void) override;

private:
	//��������ݽ��մ���
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@msg : ��Ϣ����
	void processPolledDataFromServer(
		const std::string commID,
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data);
	//���������ת������
	//@commID : ͨ��ID��ʶ
	//@flagID : Request/Response��ʶ
	//@fromID : ������ID��ʶ
	//@toID : ������ID��ʶ
	//@msg : ��Ϣ����
	void forwardPolledDataFromServer(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data);

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
	//����ID��ʶ���ϴ�����ͼƬʱʹ��
	//���ܲ�����
	std::string ownerXMQID;
	std::string parentXMQID;
};//class MDCServer

