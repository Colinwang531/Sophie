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
				const char* data = nullptr, 
				const unsigned int databytes = 0) override;

			//��ȡ�ͻ�������
			//�������ɼ̳��߶���
			//@Return : �ͻ�������
			//			����ֵ�μ�ComponentType����
			virtual int getClientType(void) const = 0;

			//��ȡ�ͻ���ID��ʶ
			//�������ɼ̳��߶���
			//@Return : �ͻ���ID��ʶ
			virtual const char* getClientID(void) const = 0;

			//��ȡ�ͻ�������
			//�������ɼ̳��߶���
			//@Return : �ͻ�������
			virtual const char* getClientName(void) const = 0;

			//���½ӿ���ʵ���߰���Ӧ��ʵ���Ĳ�ͬ���͸�����ɾ��崦��Ĺ���
			//���з����ڸ����ж�û���κ�ʵ��
			//δ֪��Ϣ����ӿ�
			virtual void unpackMessageFailure(void);
			//δ֪��Ϣ����ӿ�
			virtual void parseUnknownMessage(void);
			//������Ϣ����ӿ�
			//@pkt : ������Ϣ
			virtual void parseAlarmMessage(void* pkt = nullptr);
			//�㷨��Ϣ����ӿ�
			//@pkt : �㷨��Ϣ
			virtual void parseAlgorithmMessage(void* pkt = nullptr);
			//�����Ϣ����ӿ�
			//@pkt : �����Ϣ
			virtual void parseComponentMessage(void* pkt = nullptr);
			//��Ա��Ϣ����ӿ�
			//@pkt : ��Ա��Ϣ
			virtual void parseCrewMessage(void* pkt = nullptr);
			//�豸��Ϣ����ӿ�
			//@pkt : �豸��Ϣ
			virtual void parseDeviceMessage(void* pkt = nullptr);
			//״̬��Ϣ����ӿ�
			//@pkt : ״̬��Ϣ
			virtual void parseStatusMessage(void* pkt = nullptr);
			//�û���Ϣ����ӿ�
			//@pkt : ״̬��Ϣ
			virtual void parseUserMessage(void* pkt = nullptr);

		private:
			//�����ͻ��˵�¼�����������߳�
			//@Return : ������
			int createNewSigninAndHeartbeatWorkerThread(void);

			//��¼����������
			void signinAndHeartbeatThreadProc(void);

			//��ȡ������Ϣ���к�
			//@Return : ��Ϣ���к�
			const long long getRequestSequenceNumber(void);

			//���͵�¼������������Ϣ
			//@clientID : �ͻ���ID��ʶ,�����ID��ʶ
			//			  �ձ�ʾע��,�ǿձ�ʾ����
			//@Return : ������
			int sendSigninOrHeartbeatRequestMessage(const char* clientID = nullptr);

		private:
			bool isSigned;
			long long requestSequence;
			SharedMutex mtx;
		};//class NetworkAsynchronousClient
	}//namespace network
}//namespace base

#endif//NETWORK_ASYNCHRONOUS_CLIENT_H
