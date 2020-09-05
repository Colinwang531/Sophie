//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					��������ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-05-15									����
//										������									2020-06-29									��Ӷ�/д�ļ��ķ���
//																															���ֲ�����Ҫ���г־û�����
//

#ifndef BASE_NETWORK_NETWORK_ASYNCHRONOUS_CLIENT_H
#define BASE_NETWORK_NETWORK_ASYNCHRONOUS_CLIENT_H

#include "Mutex/RWLock.h"
#include "MessageQueue/AbstractMQClient.h"
using AbstractMQClient = mq::module::AbstractMQClient;

namespace base
{
	namespace network
	{
		class AbstractComponentClient : public AbstractMQClient
		{
		public:
			AbstractComponentClient(void);
			virtual ~AbstractComponentClient(void);

		protected:
			int startPoller(void) override;

			//��ȡ����ͻ���ID��ʶ
			//@Return : ID��ʶ
			//@Comment : �̳��߶�ID��ʶ�ĳ־û�������Ը�����ͬ,�ɼ̳���ʵ��
			virtual const std::string getAbstractComponentClientID(void) const = 0;

			//��������ͻ���ID��ʶ
			//@cid : ID��ʶ
			//@Comment : �̳��߶�ID��ʶ�ĳ־û�������Ը�����ͬ,�ɼ̳���ʵ��
			virtual void setAbstractComponentClientID(const std::string cid) = 0;

			//��ȡ����ͻ�������
			//@Return : ����
			//@Comment : �̳��߶����Ƶĳ־û�������Ը�����ͬ,�ɼ̳���ʵ��
			virtual const std::string getAbstractComponentClientName(void) const = 0;

			//��������ͻ�������
			//@name : ����
			//@Comment : �̳��߶����Ƶĳ־û�������Ը�����ͬ,�ɼ̳���ʵ��
			virtual void setAbstractComponentClientName(const std::string name) = 0;

			//��ȡ����ͻ�������
			//@Return : ����
			//			����ֵ�μ�ComponentType����
			virtual int getAbstractComponentClientType(void) const = 0;

		private:
			//����ͻ����Զ�ע��/�����߳�
			void checkAbstractComponentClientOnlineThreadProc(void);

			//��ȡ������Ϣ���к�
			//@Return : ��Ϣ���к�
			const long long getRequestSequenceNumber(void);

			//��������ͻ���ע��/����������Ϣ
			//@Return : ������
			int sendAbstractComponentClientOnlineRequestMessage(void);

		private:
			long long requestSequence;
			SharedMutex mtx;
		};//class NetworkAsynchronousClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_NETWORK_ASYNCHRONOUS_CLIENT_H
