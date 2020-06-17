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

#include "DataStruct/UnorderedMap.h"
#include "MessageQueue/AsynchronousUpstreamServer.h"
using AsynchronousUpstreamServer = mq::module::AsynchronousUpstreamServer;

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
		const char* id = nullptr, 
		const unsigned int idbytes = 0,
		const char* delimiter = nullptr, 
		const unsigned int delimiterbytes = 0,
		const char* data = nullptr, 
		const unsigned int databytes = 0) override;

	//上游服务端数据处理
	//@msg : 数据内容
	//@bytes : 数据字节数
	void afterUpstreamPollMessage(
		const char* msg = nullptr, 
		const unsigned int bytes = 0) override;

private:
	//注册组件心跳更新检测线程
	void updateComponentCheckingThreadProc(void);

	//处理组件类消息
	//@s : SOCKET标识
	//@mid : 消息ID标识
	//@idbytes : ID标识字节数
	//@pkt : 消息包
	void dealWithComponentMessage(
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr);

	//处理状态类消息
	//@s : SOCKET标识
	//@mid : 消息ID标识
	//@idbytes : ID标识字节数
	//@pkt : 消息包
	void dealWithStatusMessage(
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0,
		void* pkt = nullptr);

	//添加组件信息
	//@cname : 组件名称
	//@type : 组件类型
	//@Return : 组件UUID标识
	//			返回值空表示添加失败
	const std::string addComponentInfo(
		const std::string cname,
		const int type = 0);

	//应答添加组件信息
	//@cid : 组件ID标识
	//@result : 注册状态
	//@sequence : 消息序列号
	//@s : SOCKET标识
	//@mid : 消息ID标识
	//@idbytes : ID标识字节数
	//@Return : 错误码
	int replyAddComponentInfo(
		const std::string cid,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0);

	//删除注册组件
	//@cid : 组件ID标识
	//@type : 组件类型
	//@Return : 错误码
	//@Comment : 无应答
	int removeComponentInfo(
		const std::string cid,
		const int type = 0);

	//更新组件心跳信息
	//@cid : 组件ID标识
	//@type : 组件类型
	//@Return : 错误码
	//@Comment : 无应答
	int updateCompnentInfo(
		const std::string cid,
		const int type = 0);

	//应答查询组件信息
	//@rcis : 组件信息集合
	//@result : 注册状态
	//@sequence : 消息序列号
	//@s : SOCKET标识
	//@mid : 消息ID标识
	//@idbytes : ID标识字节数
	//@Return : 错误码
	int replyQueryComponentInfo(
		const std::vector<void*>& rcis,
		const int result = 0,
		const long long sequence = 0,
		void* s = nullptr,
		const char* mid = nullptr,
		const unsigned int idbytes = 0);

	//发送应答消息
	//@s : SOCKET标识
	//@mid : 消息ID标识
	//@idbytes : ID标识字节数
	//@msg : 消息字符串
	//@bytes : 消息字节数
	//@Return : 错误码
	int sendReplyMessage(
		void* s = nullptr, 
		const char* mid = nullptr,
		const unsigned int idbytes = 0,
		const char* msg = nullptr,
		const unsigned int bytes = 0);

private:
	//注册组件集合
	using RegisterComponentInfos = UnorderedMap<const std::string, void*>;
	RegisterComponentInfos registerComponentInfos;
	//该服务属于节点类型时以下3个属性才有效
	//这3个属性放在这里显得不是很合乎逻辑
	//true表示航行,false表示停航,默认值为true
	bool sailStatus;
	//true表示自动,false表示手动,默认值为true
	bool autoStatus;
	std::string nodeName;
};//class MDCServer

