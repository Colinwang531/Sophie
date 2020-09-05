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

#ifndef BASE_NETWORK_DEVICE_COMPONENT_CLIENT_H
#define BASE_NETWORK_DEVICE_COMPONENT_CLIENT_H

#include "Network/AbstractComponentClient.h"
using AbstractComponentClient = base::network::AbstractComponentClient;

namespace base
{
	namespace network
	{
		class DeviceComponentClient : public AbstractComponentClient
		{
		public:
			DeviceComponentClient(void);
			virtual ~DeviceComponentClient(void);

		protected:
			//���ݴ���ӿ�
			//@data : ���������ַ���
			//@databytes : ���������ֽ���
			void afterClientPollMessage(
				const void* data = nullptr,
				const unsigned int databytes = 0) override;

			//�����豸���ע��Ӧ����Ϣ
			//@cid : ���ID��ʶ
			//@Return : ������
			virtual int afterParseDeviceComponentSigninResponse(const std::string cid) = 0;

			//���������豸������Ϣ
			//@device : �豸��Ϣ
			//@Return : ������
			virtual int afterParseDeviceCommandNewRequest(void* device = nullptr) = 0;

			//����ɾ���豸������Ϣ
			//@did : �豸ID��ʶ
			//@Return : ������
//			virtual int afterParseDeviceCommandDeleteRequest(const std::string did) = 0;
		};//class DeviceComponentClient
	}//namespce network
}//namespace base

#endif//BASE_NETWORK_DEVICE_COMPONENT_CLIENT_H
