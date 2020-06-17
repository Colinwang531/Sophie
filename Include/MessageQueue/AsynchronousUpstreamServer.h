//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-30
//		Description :					�첽���η������
//
//		History:						Author									Date										Description
//										������									2020-04-30									����
//

#ifndef ASYNCHRONOUS_UPSTREAM_SERVER_H
#define ASYNCHRONOUS_UPSTREAM_SERVER_H

#include "AsynchronousServer.h"

namespace mq
{
	namespace module
	{
		class AsynchronousUpstreamServer : public AsynchronousServer
		{
		public:
			AsynchronousUpstreamServer(void);
			virtual ~AsynchronousUpstreamServer(void);

		protected:
			//�������η����ͨ��ģ��
			//@listenPort : ���ؼ����˿ں�
			//@upstreamIP : ���η����IP��ַ
			//@upstreamPort : ���η���˶˿ں�
			//@Return : ������
			int createNewModule(
				const unsigned short listenPort = 61001, 
				const char* upstreamIP = nullptr, 
				const unsigned short upstreamPort = 61101) override;

			//�������η����ģ��
			//@Return : ������ֵ
			int destroyModule(void) override;

			//�������ݶ�ȡ�߳�
			//@Return : ������
			int startPoller(void) override;

			//���ݴ���ӿ�
			//@msg : ��������
			//@bytes : �����ֽ���
			virtual void afterUpstreamPollMessage(
				const char* msg = nullptr, 
				const unsigned int bytes = 0) = 0;

		private:
			//������Ϣ�����߳�
			void pollerThreadProc(void);

		private:
			void* dealer;
		};//class AsynchronousUpstreamServer
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_UPSTREAM_SERVER_H
