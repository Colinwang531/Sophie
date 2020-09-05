//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-03
//		Description :					�ͻ��˳�����
//
//		History:						Author									Date										Description
//										������									2020-05-03									����
//										������									2020-08-17									1.����Ϊ�������
//										     									          									2.������������ͱ�ʶ
//										     									          									3.��������ʵ����ʵ���ӿ�
//

#ifndef BASE_NETWORK_ABSTRACT_CLIENT_H
#define BASE_NETWORK_ABSTRACT_CLIENT_H

namespace base
{
	namespace network
	{
		typedef enum class tagClientModuleType_t : int
		{
			CLIENT_MODULE_TYPE_NONE = 0,
			CLIENT_MODULE_TYPE_MAJORDOMO_WORKER,
			CLIENT_MODULE_TYPE_MAJORDOMO_CLIENT,
		}ClientModuleType;

		class AbstractClient
		{
		public:
			AbstractClient(
				const ClientModuleType type = ClientModuleType::CLIENT_MODULE_TYPE_NONE);
			virtual ~AbstractClient(void);

		public:
			//�����ͻ���
			//@address : ����˵�ַ
			//@name : ���������
			//@Return : ������
			int startClient(
				const std::string address,
				const std::string name);

			//ֹͣ�ͻ���
			//@Return : ������
			int stopClient(void);

			//��ȡ�ͻ�������/ֹͣ״̬
			//@Return : �ͻ�������/ֹͣ״̬
			//@Comment : �ͻ�������/ֹͣ״ֻ̬���ɷ�����ڲ��޸�,�ⲿֻ�ܻ�ȡ��ֵ
			inline const bool isStopped(void) const
			{
				return stopped;
			}

			//������Ϣ
			//@flagID : Request/Response��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@msg : ��Ϣ����
			//@Return : ������
			int sendMessageData(
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

			//�ͻ������ݽ��մ���
			//@flagID : Request/Response��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@msg : ��Ϣ����
			virtual void afterClientPolledMessageProcess(
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg) = 0;
			virtual const std::string buildAutoRegisterToBrokerMessage(void) = 0;

		protected:
			//�����ͻ���ģ��
			//@address : ����˵�ַ
			//@name : ���������
			//@Return : ������
			virtual int createNewClientModule(
				const std::string address,
				const std::string name);

			//���ٿͻ���ģ��
			//@Return : ������
			virtual int destroyClientModule(void);
			
		protected:
			const ClientModuleType clientModuleType;
			void* clientModule;
			bool stopped;
		};//class AbstractClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_CLIENT_H
