//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-07-06
//		Description :					AIS类
//
//		History:						Author									Date										Description
//										王科威									 2021-07-06									 创建
//

#ifndef AIS_H
#define AIS_H

#include "libnetwork/zmq/module/worker.h"
using Worker = framework::libnetwork::zmq::module::Worker;
#include "liblog/log.h"
using Log = framework::liblog::Log;
#include "libcommon/hardware/com/com.h"
using Com = framework::libcommon::hardware::Com;

class AIS final : protected Worker
{
public:
	AIS(
		Log& log, 
		const std::string appID,
		void* ctx = nullptr);
	~AIS(void);

public:
	//启动
	//@remoteIP : 远程服务IP
	//@remotePort : 远程服务端口
	//@number : 串口号
	//@baudrate : 波特率
	//@Return : 错误码
	int start(
		const std::string remoteIP,
		const unsigned short remotePort,
		const int number = -1,
		const int baudrate = 4800);

	//停止
	int stop(void);

protected:
	void afterWorkerPollDataProcess(const std::string data) override;
	void afterComReadDataHandler(
		const char* data = nullptr,
		const int bytes = 0);

private:
	void timerTaskThreadHandler(void);
	void sendRegisterMessage(void);
	void sendQueryMessage(void);
	void sendNotifyMessage(const std::string data);
	void processRegisterMessage(void* parser = nullptr);
	void processQueryMessage(void* parser = nullptr);

private:
	Com com;
	Log& logObj;
	const std::string applicationID;
	const std::string sourceID;
	std::string aisData;
	std::string parseData;
	std::string webApplicationID;
	void* tid;
	bool stopped;
};//class AIS

#endif//AIS_H
