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
			//�������η��������ģ��
			//@port : ���η���˶˿ں�
			//@upstream : ���η�������ӵ�ַ
			//@Return : ������ֵ
			int createNewModule(const unsigned short port = 61001, const char* address = nullptr) override;

			//���ٷ����ģ��
			//@Return : ������ֵ
			int destroyModule(void) override;

			//������ݶ�ȡʵ����
			//@items : ���ݶ�ȡʵ������
			void addPollItem(std::vector<void*>& items) override;

			//���ݶ�ȡ����
			//@s : ���ݶ�ȡ��socketʵ��
			void afterPollItemMessage(void* s = nullptr) override;

		private:
			void* dealer;
		};//class AsynchronousUpstreamServer
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_UPSTREAM_SERVER_H
