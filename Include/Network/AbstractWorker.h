//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-15
//		Description :					�ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-09-15									����
//

#ifndef BASE_NETWORK_ABSTRACT_WORKER_H
#define BASE_NETWORK_ABSTRACT_WORKER_H

#include "AbstractClient.h"

namespace base
{
	namespace network
	{
		class AbstractWorker : public AbstractClient
		{
		public:
			AbstractWorker(void);
			virtual ~AbstractWorker(void);

		public:
			//��ȡUUID��ʶ
			//@Return : UUID��ʶ
			inline const std::string getUUID(void) const
			{
				return uuid;
			}

			//����UUID��ʶ
			//@name : ��������
			//@Comment : ��ȡConfig.xml�е�ID��ʶ,���ʧ���򴴽��µ�UUID��ʶ
			void generateUUIDWithName(const std::string name);

		protected:
			int createNewClient(const std::string) override;
			//�����Զ�ע��/����������Ϣ
			virtual void sendRegisterWorkerServerMessage(void) = 0;
			virtual void sendQuerySystemServiceMessage(void);

		private:
			//����ʱ�����߳�
			void processTaskTimerThread(void);

 		private:
			//����UUID��ò��϶�ȡ�ļ�
			std::string uuid;
		};//class AbstractMediaStreamClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_WORKER_H
