//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					�����豸����ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//

#include "Network/NetworkAsynchronousClient.h"
using NetworkAsynchronousClient = base::network::NetworkAsynchronousClient;

class HKDCClient : public NetworkAsynchronousClient
{
public:
	HKDCClient(const std::string cname);
	~HKDCClient(void);

protected:
	int getClientType(void) const override;
	const char* getClientID(void) const override;
	const char* getClientName(void) const override;
	void unpackMessageFailure(void) override;
	void parseUnknownMessage(void) override;
	void parseComponentMessage(void* pkt = nullptr) override;

private:
	//����ComponentType
	const int clientType;
	//�������ID��ʶ
	std::string clientID;
	std::string clientName;
};//class HKDCClient

