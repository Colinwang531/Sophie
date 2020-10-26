//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-13
//		Description :					消息分发异步服务端类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
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
	//服务端数据接收处理
	//@commID : 通信ID标识
	//@flagID : Request/Response标识
	//@fromID : 发送者ID标识
	//@toID : 接收者ID标识
	//@msg : 消息数据
	void processPolledDataFromServer(
		const std::string commID,
		const std::string roleID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data);
	//服务端数据转发处理
	//@commID : 通信ID标识
	//@flagID : Request/Response标识
	//@fromID : 发送者ID标识
	//@toID : 接收者ID标识
	//@msg : 消息数据
	void forwardPolledDataFromServer(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string data);

	//处理组件消息
	//@commID : 通信ID标识
	//@flagID : Request/Response标识
	//@fromID : 发送者ID标识
	//@pkt : 消息数据
	void processComponentMessage(
		const std::string commID, 
		const std::string flagID,
		const std::string fromID,
		DataPacketPtr pkt);

	//添加新注册的组件
	//@componentID : 组件ID标识
	//@serviceName : 组件服务名称
	//@communicationID : 通信ID标识
	//@componentType : 组件类型
	//@Return : 错误码
	//@Comment : 通过replyAddNewRegisterComponent方法应答
	int addNewRegisterComponent(
		const std::string componentID,
		const std::string serviceName,
		const std::string communicationID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

	//更新组件信息
	//@componentID : 组件ID标识
	//@serviceName : 组件名称
	//@communicationID : 通信ID标识
	//@componentType : 组件类型
	//@Return : 错误码
	//@Comment : 无应答
	int updateRegisterCompnent(
		const std::string componentID,
		const std::string serviceName,
		const std::string communicationID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

	//删除组件
	//@componentID : 组件ID标识
	//@Return : 错误码
	int removeComponentByID(const std::string componentID);

	//服务端应答添加组件信息
	//@cid : 组件ID标识
	//@result : 注册状态
	//@sequence : 消息序列号
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@Return : 错误码
	int replyMessageWithResultAndExtendID(
		const std::string commID,
		const std::string extendID,
		const std::string fromID,
		const int pkttype = 0,
		const int command = 0,
		const int result = 0,
		const long long sequence = 0);

private:
	//注册组件集合
	//KEY是组件ID标识
	using RegisterComponentGroup = UnorderedMap<const std::string, AbstractComponentPtr>;
	RegisterComponentGroup registerComponentGroup;
	//自身ID标识在上传报警图片时使用
	//但很不合理
	std::string ownerXMQID;
	std::string parentXMQID;
};//class MDCServer

