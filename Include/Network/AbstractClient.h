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

#include <string>

namespace base
{
	namespace network
	{
		class AbstractClient
		{
		public:
			AbstractClient(void);
			virtual ~AbstractClient(void);

		public:
			//�����ͻ���
			//@address : ��������ӵ�ַ
			//@Return : ������
			int startClient(const std::string address);

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

			//��������
			//@roleID : ��ɫID��ʶ
			//@flagID : ��־ID��ʶ
			//@fromID : ������ID��ʶ
			//@toID : ������ID��ʶ
			//@data : ��Ϣ����
			//@Return : ������
			virtual int sendData(
				const std::string roleID,
				const std::string flagID,
				const std::string fromID,
				const std::string toID,
				const std::string data) = 0;

		protected:
			//�����ͻ���
			//@address : ����˵�ַ
			//@Return : ������
			virtual int createNewClient(
				const std::string address) = 0;

			//���ٿͻ���
			//@Return : ������
			virtual int destroyClient(void) = 0;
			
		protected:
			bool stopped;
		};//class AbstractClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_ABSTRACT_CLIENT_H
