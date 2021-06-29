//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-12
//		Description :					消息交换应用类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-12									 创建
//

#ifndef XMQ_H
#define XMQ_H

#include "libnetwork/zmq/module/switcher.h"
using Switcher = framework::libnetwork::zmq::module::Switcher;
#include "libnetwork/zmq/module/worker.h"
using Worker = framework::libnetwork::zmq::module::Worker;
#include "liblog/log.h"
using Log = framework::liblog::Log;

class XMQ final : protected Switcher, protected Worker
{
public:
	XMQ(
		Log& log, 
		const std::string appID, 
		void* ctx = nullptr);
	~XMQ(void);

public:	
	//监听
	//@localIP : 监听IP
	//@localPort : 监听端口
	//@Return : 错误码
	int bind(
		const std::string localIP,
		const unsigned short localPort = 0);

	//连接
	//@remoteIP : 远程服务IP
	//@remotePort : 远程服务端口
	//@Return : 错误码
	int connect(
		const std::string remoteIP,
		const unsigned short remotePort = 0);

protected:
	void afterSwitcherPollDataProcess(
		const std::string sourceID, 
		const std::string data) override;
	void afterWorkerPollDataProcess(
		const std::string data) override;

private:
	//转发消息给Via
	//@parser : 解析实例
	//@Return : true表示处理成功,否则失败
	bool forwardVia(void* parser = nullptr);

	//转发消息给Upload
	//@parser : 解析实例
	//@Return : true表示处理成功,否则失败
	bool forwardUpload(void* parser = nullptr);

	//转发消息给Receiver
	//@parser : 解析实例
	//@Return : true表示处理成功,否则失败
	bool forwardReceiver(void* parser = nullptr);

	//转发消息给Friend
	//@parser : 解析实例
	//@Return : true表示处理成功,否则失败
	void forwardFriend(const std::string data);

	//处理pair消息
	void processPairMessage(
		const std::string sourceID, 
		void* parser = nullptr);

	//处理setup消息
	void processSetupMessage(void* parser = nullptr);

private:
	Log& logObj;
	const std::string applicationID;
	std::string sourceID;
	std::string friendID;
};//class XMQ

#endif//XMQ_H
