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

namespace base
{
	namespace network
	{
		class AbstractServer
		{
		public:
			AbstractServer(void);
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

			//��������
			//@commID : ͨ��ID��ʶ
			//@roleID : ��ɫID��ʶ
			//@flagID : ��־ID��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@data : ��Ϣ����
			//@Return : ������
			virtual int sendData(
				const std::string commID,
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data) = 0;

			//�ͻ�������/���߳�ʱ����
			virtual void afterAutoClientConnectionTimeoutProcess(void) = 0;

		protected:
			//�������ط����
			//@address : ���ؼ�����ַ
			//@Return : ������
			virtual int createNewServer(
				const std::string address) = 0;

			//���ٱ��ط����
			//@Return : ������
			virtual int destroyServer(void) = 0;

		private:
			//�ͻ�������/���߼���߳�
			void autoCheckClientConnectionTimeoutThread(void);

		protected:
			bool stopped;
		};//class AbstractServer
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_SERVER_H
