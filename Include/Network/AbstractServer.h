//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					����˳�����
//
//		History:						Author									Date										Description
//										������									2020-04-26									����
//										������									2020-08-17									1.����Ϊ�������
//										     									          									2.������������ͱ�ʶ
//										     									          									3.��������ʵ����ʵ���ӿ�
//

#ifndef BASE_NETWORK_ABSTRACT_SERVER_H
#define BASE_NETWORK_ABSTRACT_SERVER_H

#include <vector>

namespace base
{
	namespace network
	{
		typedef enum class tagServerModuleType_t : int
		{
			SERVER_MODULE_TYPE_NONE = 0,
			SERVER_MODULE_TYPE_ASIO,
			SERVER_MODULE_TYPE_MAJORDOMO_BROKER,
			SERVER_MODULE_TYPE_ASYNC_DISPATHER
		}ServerModuleType;

		class AbstractServer
		{
		public:
			AbstractServer(
				const ServerModuleType type = ServerModuleType::SERVER_MODULE_TYPE_NONE);
			virtual ~AbstractServer(void);

		public:
			//���������
			//@listenPort : ���ؼ����˿ں�
			//@Return : ������
			int startServer(const unsigned short listenPort = 61001);

			//ֹͣ�����
			//@Return : ������
 			int stopServer(void);

			//��ȡ���������/ֹͣ״̬
			//@Return : ���������/ֹͣ״̬
			//@Comment : ���������/ֹͣ״ֻ̬���ɷ�����ڲ��޸�,�ⲿֻ�ܻ�ȡ��ֵ
			inline const bool isStopped(void) const
			{
				return stopped;
			}

			//������Ϣ
			//@commID : ͨ��ID��ʶ
			//@flagID : Request/Response��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@msg : ��Ϣ����
			//@Return : ������
			int sendMessageData(
				const std::string commID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg);

			//���ӳ�ʱ����
			virtual void afterAutoCheckConnectionTimeoutProcess(void) = 0;

			//��������ݽ��մ���
			//@commID : ͨ��ID��ʶ
			//@flagID : Request/Response��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@msg : ��Ϣ����
			virtual void afterServerPolledMessageProcess(
				const std::string commID, 
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string msg) = 0;

		protected:
			//�������ط����ģ��
			//@address : ���ؼ�����ַ
			//@Return : ������
			virtual int createNewServerModule(const std::string address);

			//���ٱ��ط����ģ��
			//@Return : ������
			virtual int destroyServerModule(void);

		protected:
			const ServerModuleType serverModuleType;
			void* serverModule;
			bool stopped;
		};//class AbstractServer
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_SERVER_H
