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

#include "boost/function.hpp"
#include "Network/NetworkAsynchronousClient.h"
using NetworkAsynchronousClient = base::network::NetworkAsynchronousClient;

typedef boost::function<
	int(
		const std::string,//ID
		const std::string,//IP
		const unsigned short,//Port
		const std::string,//Name
		const std::string//Password
		)> CreateNewDeviceCallback;
typedef boost::function<
	int(
		const std::string//ID
		)> DestroyDeviceCallback;

class HKDCClient : public NetworkAsynchronousClient
{
public:
	HKDCClient(
		const std::string cid,
		const std::string cname,
		CreateNewDeviceCallback ccb,
		DestroyDeviceCallback ddc);
	~HKDCClient(void);

protected:
	int getClientType(void) const override;
	const char* getClientID(void) const override;
	const char* getClientName(void) const override;
	void unpackMessageFailure(void) override;
	void parseUnknownMessage(void) override;
	void parseComponentMessage(void* pkt = nullptr) override;
	void parseDeviceMessage(void* pkt = nullptr) override;

private:
	//就是ComponentType
	const int clientType;
	//就是组件ID标识
	std::string clientID;
	std::string clientName;
	CreateNewDeviceCallback createNewDeviceCallback;
	DestroyDeviceCallback destroyDeviceCallback;
};//class HKDCClient

