//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-28
//		Description :					�豸����ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-06-28									����
//

#include "Network/NetworkAsynchronousClient.h"
using NetworkAsynchronousClient = base::network::NetworkAsynchronousClient;

namespace base
{
	namespace network
	{
		class DeviceComponentClient : public NetworkAsynchronousClient
		{
		public:
			DeviceComponentClient(void);
			virtual ~DeviceComponentClient(void);

		protected:
			void parseAlarmMessage(void* pkt = nullptr) override {};
			void parseAlgorithmMessage(void* pkt = nullptr) override {};
			void parseComponentMessage(void* pkt = nullptr) override;
			void parseCrewMessage(void* pkt = nullptr) override {};
			void parseDeviceMessage(void* pkt = nullptr) override;
			void parseStatusMessage(void* pkt = nullptr) override {};
			void parseUserMessage(void* pkt = nullptr) override {};

			//���������豸������Ϣ
			//@did : �豸ID��ʶ
			//@address : �豸IP��ַ
			//@port : �豸�˿ں�
			//@name : �豸��¼����
			//@password : �豸��¼����
			//@Return : ������
			virtual int afterParseDeviceCommandNewRequest(
				const std::string did, 
				const std::string address,
				const unsigned short port,
				const std::string name,
				const std::string password) = 0;

			//����ɾ���豸������Ϣ
			//@did : �豸ID��ʶ
			//@Return : ������
			virtual int afterParseDeviceCommandDeleteRequest(const std::string did) = 0;
		};//class DeviceComponentClient
	}//namespce network
}//namespace base

