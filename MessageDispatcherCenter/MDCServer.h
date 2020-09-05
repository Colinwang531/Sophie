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

	//服务端数据接收处理
	//@commID : 通信ID标识
	//@flagID : Request/Response标识
	//@fromID : 发送者ID标识
	//@toID : 接收者ID标识
	//@msg : 消息数据
	void processServerPolledMessage(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg);
	//服务端数据转发处理
	//@commID : 通信ID标识
	//@flagID : Request/Response标识
	//@fromID : 发送者ID标识
	//@toID : 接收者ID标识
	//@msg : 消息数据
	void forwardServerPolledMessage(
		const std::string commID,
		const std::string flagID,
		const std::string fromID,
		const std::string toID,
		const std::string msg);

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

	//转发状态配置消息
	//@fwdmsg : 消息数据
	//@commID : 通信ID标识
	//@status : 请求消息
	//@sequence : 请求消息序列号
// 	void forwardStatusConfigureMessage(
// 		const std::string fwdmsg,
// 		const std::string commID,
// 		void* status = nullptr,
// 		const long long sequence = -1);

	//转发设备消息
	//@commID : 通信ID标识
	//@pkt : 请求消息
	//@msg : 消息数据
// 	void forwardDeviceMessage(
// 		const std::string commID,
// 		DataPacketPtr pkt,
// 		const std::string msg);

	//转发算法消息
	//@fwdmsg : 消息数据
	//@commID : 通信ID标识
	//@algorithm : 请求消息
	//@sequence : 请求消息序列号
// 	void forwardAlgorithmConfigureMessage(
// 		const std::string fwdmsg,
// 		const std::string commID,
// 		void* algorithm = nullptr,
// 		const long long sequence = -1);

	//转发报警数据消息
	//@s : SOCKET标识
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
// 	void forwardAlarmInfoMessage(
// 		void* s = nullptr,
// 		const void* data = nullptr,
// 		const unsigned int databytes = 0);

	//转发成员消息
	//@fwdmsg : 消息数据
	//@commID : 通信ID标识
	//@crew : 请求消息
	//@sequence : 请求消息序列号
// 	void forwardCrewConfigureMessage(
// 		const std::string fwdmsg,
// 		const std::string commID,
// 		void* crew = nullptr,
// 		const long long sequence = -1);

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

	//删除注册组件
	//@componentID : 组件ID标识
	//@componentType : 组件类型
	//@Return : 错误码
	//@Comment : 无应答
	int removeRegisterComponent(
		const std::string componentID,
		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

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

	//向指定的组件转发消息
	//@fwdmsg : 消息内容
	//@componentType : 组件类型
	//@Return : 错误码
// 	int forwardMessageByComponentType(
// 		const std::string fwdmsg,
// 		const base::component::ComponentType componentType = base::component::ComponentType::COMPONENT_TYPE_NONE);

private:
	//注册组件集合
	//key是通信ID标识
	using RegisterComponentGroup = UnorderedMap<const std::string, AbstractComponentPtr>;
	RegisterComponentGroup registerComponentGroup;
};//class MDCServer

