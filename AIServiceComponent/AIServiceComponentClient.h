//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					海康设备组件客户端类
//
//		History:						Author									Date										Description
//										王科威									2020-05-13									创建
//

#include "Device/Hikvision/HikvisionDevice.h"
using AbstractDevicePtr = boost::shared_ptr<base::device::AbstractDevice>;
#include "DataStruct/UnorderedMap.h"
using AbstractDeviceGroup = UnorderedMap<const std::string, AbstractDevicePtr>;
#include "Network/DeviceComponentClient.h"
using DeviceComponentClient = base::network::DeviceComponentClient;

class AlgorithmComponentClient : public DeviceComponentClient
{
public:
	AlgorithmComponentClient(void);
	~AlgorithmComponentClient(void);

protected:
	const std::string getClientID(void) const override;
	int setClientID(const std::string ID) const override;
	const std::string getClientName(void) const override;
	int setClientName(const std::string name) const override;
	int getClientType(void) const override;
	void unpackMessageFailure(void) override;
	void parseUnknownMessage(void) override;

	//处理新增设备请求消息
	//@did : 设备ID标识
	//@address : 设备IP地址
	//@port : 设备端口号
	//@name : 设备登录名称
	//@password : 设备登录密码
	//@Return : 错误码
	int afterParseDeviceCommandNewRequest(
		const std::string did,
		const std::string address,
		const unsigned short port,
		const std::string name,
		const std::string password) override;

	//处理删除设备请求消息
	//@did : 设备ID标识
	//@Return : 错误码
	int afterParseDeviceCommandDeleteRequest(const std::string did) override;

private:
	AbstractDeviceGroup abstractDeviceGroup;
};//class HKDComponentClient

