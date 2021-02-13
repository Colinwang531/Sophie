//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-20
//		Description :					�豸������
//
//		History:						Author									Date										Description
//										������									2021-01-20									����
//

#ifndef FRAMEWORK_DEVICE_DEVICE_FACTORY_H
#define FRAMEWORK_DEVICE_DEVICE_FACTORY_H

#include <vector>
#include "boost/serialization/singleton.hpp"
#include "boost/shared_ptr.hpp"
#include "Device/Device.h"
using Device = framework::device::Device;
using DevicePtr = boost::shared_ptr<Device>;

namespace framework
{
	namespace device
	{
		typedef struct tagCameraInfos_t 
		{
			bool enable;
			int type;//0ģ��,1����
			int idx;
			std::string id;
			std::string ipv4;
		}CameraInfos;

		class DeviceFactory
			: public boost::serialization::singleton<DeviceFactory>
		{
		public:
			DeviceFactory(void);
			~DeviceFactory(void);

		public:
			//�����豸
			//@id : �豸ID��ʶ
			//@factory : �豸����
			//@type : �豸����
			//@Return : �豸ʵ��
			DevicePtr createNewDevice(
				const std::string id,
				const FactoryType ft = FactoryType::FACTORY_TYPE_NONE,
				const DeviceType dt = DeviceType::DEVICE_TYPE_NONE);

			//��¼�豸
			//@device : �豸
			//@username : ��¼�û���
			//@userpwd : ��¼����
			//@ipv4 : �豸IPv4��ַ
			//@port : �豸�˿ں�
			//@Return : ������
			int loginDevice(
				DevicePtr device,
				const std::string username,
				const std::string userpwd,
				const std::string ipv4,
				const unsigned short port = 0);

			//�ǳ��豸
			//@device : �豸
			//@Return : ������
			int logoutDevice(DevicePtr device);

			//��ȡ�������Ϣ
			//@device : �豸
			//@cinfos[out] : �������Ϣ
			//@Return : ������
			int getCameraInfos(DevicePtr device, std::vector<CameraInfos>& cinfos);
		};//class DeviceFactory
	}//namespace device
}//namespace framework

#endif//FRAMEWORK_DEVICE_DEVICE_FACTORY_H
