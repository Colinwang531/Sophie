//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					�첽ͨ�ſͻ�����
//
//		History:						Author									Date										Description
//										������									2020-05-15									����
//										������									2020-06-29									��Ӷ�/д�ļ��ķ���
//																															���ֲ�����Ҫ���г־û�����
//

#ifndef NETWORK_ASYNCHRONOUS_CLIENT_H
#define NETWORK_ASYNCHRONOUS_CLIENT_H

#include "Mutex/RWLock.h"
#include "MessageQueue/AsynchronousClient.h"
using AsynchronousClient = mq::module::AsynchronousClient;

namespace base
{
	namespace network
	{
		class NetworkAsynchronousClient : public AsynchronousClient
		{
		public:
			NetworkAsynchronousClient(void);
			virtual ~NetworkAsynchronousClient(void);

		protected:
			int startPoller(void) override;

			//���ݴ���ӿ�
			//@data : ���������ַ���
			//@databytes : ���������ֽ���
			void afterClientPollMessage(
				const void* data = nullptr, 
				const unsigned int databytes = 0) override;

			//��ȡ�ͻ���ID��ʶ,�����ID��ʶ
			//@Return : ����ֵ
			virtual const std::string getClientID(void) const = 0;

			//���ÿͻ���ID��ʶ
			//@value : ����ֵ
			//@Return : ������
			virtual int setClientID(const std::string ID) const = 0;

			//��ȡ�ͻ�������
			//@Return : ����ֵ
			virtual const std::string getClientName(void) const = 0;

			//���ÿͻ�������
			//@value : ����ֵ
			//@Return : ������
			virtual int setClientName(const std::string name) const = 0;

			//��ȡ�ͻ�������,���������
			//�����ɼ̳���ָ��,�����ṩ���÷���
			//@Return : �ͻ�������
			//			����ֵ�μ�ComponentType����
			virtual int getClientType(void) const = 0;

			//�ͻ�����Ϣ��������
			//@pkt : ��Ϣ��
			virtual void unpackMessageFailure(void) = 0;
			virtual void parseUnknownMessage(void) = 0;
			virtual void parseAlarmMessage(void* pkt = nullptr) = 0;
			virtual void parseAlgorithmMessage(void* pkt = nullptr) = 0;
			virtual void parseComponentMessage(void* pkt = nullptr) = 0;
			virtual void parseCrewMessage(void* pkt = nullptr) = 0;
			virtual void parseDeviceMessage(void* pkt = nullptr) = 0;
			virtual void parseStatusMessage(void* pkt = nullptr) = 0;
			virtual void parseUserMessage(void* pkt = nullptr) = 0;

		private:
			//�����ͻ��˵�¼�����������߳�
			//@Return : ������
			int createNewSigninAndHeartbeatWorkerThread(void);

			//��¼����������
			void signinAndHeartbeatThreadProc(void);

			//��ȡ������Ϣ���к�
			//@Return : ��Ϣ���к�
			const long long getRequestSequenceNumber(void);

			//����ע�������������Ϣ
			//@Return : ������
			int sendSigninOrHeartbeatRequestMessage(void);

		private:
			long long requestSequence;
			SharedMutex mtx;
		};//class NetworkAsynchronousClient
	}//namespace network
}//namespace base

#endif//NETWORK_ASYNCHRONOUS_CLIENT_H
