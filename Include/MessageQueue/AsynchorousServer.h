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

#ifndef ABSTRACT_SERVER_H
#define ABSTRACT_SERVER_H

#include <vector>

namespace base
{
	namespace network
	{
		typedef enum class tagNetworkServerType_t : int
		{
			NETWORK_SERVER_TYPE_NONE = 0,
			NETWORK_SERVER_TYPE_ASIO,
			NETWORK_SERVER_TYPE_MAJORDOMO_BROKER,
			NETWORK_SERVER_TYPE_ASYNC_DISPATHER
		}NetworkServerType;

		class AbstractServer
		{
		public:
			AbstractServer(
				const NetworkServerType type = NetworkServerType::NETWORK_SERVER_TYPE_NONE);
			virtual ~AbstractServer(void);

		public:
			//���������
			//@listenPort : ���ؼ����˿ں�
			//@upstreamIP : ���η����IP��ַ
			//@upstreamPort : ���η���˶˿ں�
			//@Return : ������
			//@Comment : �������Ҫ�������η����ʱ,upstreamIP��upstreamPort��2�������ǿ��Բ����õ�
			int startServer(
				const unsigned short listenPort = 61001, 
				const char* upstreamIP = nullptr, 
				const unsigned short upstreamPort = 61101);

			//ֹͣ�����
			//@Return : ������
 			int stopServer(void);

		protected:
			//�������ط����ͨ��ģ��
			//@listenPort : ���ؼ����˿ں�
			//@Return : ������
			//@Comment : listenPort���������ڸ����ж���,���ڱ��ؼ����˿�
			//			 �ڶ����͵����������������ඨ��,�����������ӵ�ַ�Ͷ˿�
			virtual int createNewModule(
				const unsigned short listenPort = 61001, 
				const char* = nullptr, 
				const unsigned short = 0);

			//���ٷ����ģ��
			//@Return : ������
			virtual int destroyModule(void);

			//�������ݶ�ȡ�߳�
			//@Return : ������
			virtual int startPoller(void);

			//���ݴ���ӿ�
			//@s : SOCKET��ʶ
			//@id : ID��ʶ�ַ���
			//@idbytes : ID��ʶ�ֽ���
			//@delimiter : �ָ����ַ���
			//@delimiterbytes : �ָ����ֽ���
			//@data : ���������ַ���
			//@databytes : ���������ֽ���
			//@Comment : ���ݴ����ʵ�ֱ��������������,���಻�����κ�����
			virtual void afterServerPollMessage(
				void* s = nullptr,
				const void* id = nullptr, 
				const unsigned int idbytes = 0,
				const void* delimiter = nullptr, 
				const unsigned int delimiterbytes = 0,
				const void* data = nullptr, 
				const unsigned int databytes = 0) = 0;

		private:
			//��Ϣת���߳�
			void pollerThreadProc(void);

			//�����첽����˴����߳�
			//@Return : ������
			int startWorker(void);

			//��Ϣ�����߳�
			void workerThreadProc(void);

			//��Ϣ�ַ�����
			//@src : ����ԴID
			//@dest : Ŀ��ID
			void transferData(void* src = nullptr, void* dest = nullptr);

		protected:
			void* ctx;
			//�߳��˳���ʶ
			bool stopped;

		private:
			void* router;
			void* dealer;
		};//class AsynchronousServer
	}//namespace module
}//namespace mq

#endif//ABSTRACT_SERVER_H
