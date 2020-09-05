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

	//���������豸������Ϣ
	//@did : �豸ID��ʶ
	//@address : �豸IP��ַ
	//@port : �豸�˿ں�
	//@name : �豸��¼����
	//@password : �豸��¼����
	//@Return : ������
	int afterParseDeviceCommandNewRequest(
		const std::string did,
		const std::string address,
		const unsigned short port,
		const std::string name,
		const std::string password) override;

	//����ɾ���豸������Ϣ
	//@did : �豸ID��ʶ
	//@Return : ������
	int afterParseDeviceCommandDeleteRequest(const std::string did) override;

private:
	AbstractDeviceGroup abstractDeviceGroup;
};//class HKDComponentClient

