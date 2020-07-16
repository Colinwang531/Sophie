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
#include "MessageQueue/AsynchronousUpstreamServer.h"
using AsynchronousUpstreamServer = mq::module::AsynchronousUpstreamServer;
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
using AbstractComponentPtr = boost::shared_ptr<AbstractComponent>;

class MDCServer : public AsynchronousUpstreamServer
{
public:
	MDCServer(void);
	~MDCServer(void);

protected:
	//启动数据读取线程
	//@Return : 错误码
	int startPoller(void) override;

	//本地服务端数据处理
	//@s : SOCKET标识
	//@id : ID标识字符串
	//@idbytes : ID标识字节数
	//@delimiter : 分隔符字符串
	//@delimiterbytes : 分隔符字节数
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
	//@Comment : 数据处理的实现必须由派生类完成,基类不处理任何数据
	void afterServerPollMessage(
		void* s = nullptr,
		const void* id = nullptr, 
		const unsigned int idbytes = 0,
		const void* delimiter = nullptr, 
		const unsigned int delimiterbytes = 0,
		const void* data = nullptr, 
		const unsigned int databytes = 0) override;

	//上游服务端数据处理
	//@msg : 数据内容
	//@bytes : 数据字节数
	void afterUpstreamPollMessage(
		const void* msg = nullptr, 
		const unsigned int bytes = 0) override;

private:
	//自动检测组件离线状态线程
	void autoCheckOfflinStatusOfComponentThreadProc(void);

	//处理组件类消息
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@pkt : 消息包
	void dealWithComponentMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr);

	//转发状态配置消息
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@pkt : 消息包
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
	void forwardStatusConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//转发设备配置消息
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@pkt : 消息包
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
	void forwardDeviceConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//转发算法配置消息
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@pkt : 消息包
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
	void forwardAlgorithmConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//转发报警数据消息
	//@s : SOCKET标识
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
	void forwardAlarmInfoMessage(
		void* s = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//转发成员信息配置消息
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@pkt : 消息包
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
	void forwardCrewConfigureMessage(
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//添加新注册的组件
	//@c : 组件实例
	void addRegisterComponent(void* c = nullptr);

	//服务端应答添加组件信息
	//@cid : 组件ID标识
	//@result : 注册状态
	//@sequence : 消息序列号
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@Return : 错误码
	int replyAddRegisterComponent(
		const std::string cid,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0);

	//删除注册组件
	//@cid : 组件ID标识
	//@type : 组件类型
	//@Return : 错误码
	//@Comment : 无应答
	int removeRegisterComponent(
		const std::string cid,
		const int type = 0);

	//更新组件信息
	//@c : 组件实例
	//@Return : 错误码
	//@Comment : 无应答
	int updateRegisterCompnent(void* c = nullptr);

	//应答组件查询请求
	//@componentPtrs : 组件集合
	//@result : 注册状态
	//@sequence : 消息序列号
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@Return : 错误码
	int replyQueryRegisterComponent(
		const std::vector<AbstractComponentPtr>& componentPtrs,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0);

	//服务端应答设置类失败请求消息
	//@command : 命令类型
	//@sequence : 消息序列号
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@Return : 错误码
	int replyConfigureSetFailedMessage(
		const int command = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const void* commID = nullptr,
		const unsigned int idbytes = 0);

	//发送服务端应答消息
	//@s : SOCKET标识
	//@commID : 通信ID标识
	//@idbytes : ID标识字节数
	//@data : 消息字符串
	//@databytes : 消息字节数
	//@Return : 错误码
	int sendServerResponseMessage(
		void* s = nullptr, 
		const void* commID = nullptr,
		const unsigned int idbytes = 0,
		const void* data = nullptr,
		const unsigned int databytes = 0);

	//转发配置相类请求和应答消息
	//@s : SOCKET标识
	//@type : 目的端组件类型
	//@data : 数据内容字符串
	//@databytes : 数据内容字节数
	//@Return : 错误码
	int forwardConfigureRequestOrResponseMessage(
		void* s = nullptr, 
		const int type = 0, 
		const void* data = nullptr, 
		const unsigned int databytes = 0);

private:
	//注册组件集合
	using RegisterComponentGroup = UnorderedMap<const std::string, AbstractComponentPtr>;
	RegisterComponentGroup registerComponentGroup;
	//该服务属于节点类型时以下3个属性才有效
	//这3个属性放在这里显得不是很合乎逻辑
	//true表示航行,false表示停航,默认值为true
	bool sailStatus;
	//true表示自动,false表示手动,默认值为true
	bool autoStatus;
	std::string nodeName;
};//class MDCServer

