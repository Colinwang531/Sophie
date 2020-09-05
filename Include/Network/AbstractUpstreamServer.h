//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-30
//		Description :					���η���˳�����
//
//		History:						Author									Date										Description
//										������									2020-04-30									����
//										������									2020-08-18									1.����Ϊ�������
//										     									          									2.������������ͱ�ʶ
//

#ifndef BASE_NETWORK_UPSTREAM_SERVER_H
#define BASE_NETWORK_UPSTREAM_SERVER_H

#include "AbstractServer.h"
#include "AbstractClient.h"

namespace base
{
	namespace network
	{
		class AbstractUpstreamServer : public AbstractServer, public AbstractClient
		{
		public:
			AbstractUpstreamServer(
				const ServerModuleType server = ServerModuleType::SERVER_MODULE_TYPE_NONE,
				const ClientModuleType upstream = ClientModuleType::CLIENT_MODULE_TYPE_NONE,
				const std::string address = "tcp:\\127.0.0.1:61001");
			virtual ~AbstractUpstreamServer(void);

		protected:
			//�������ط����ģ��
			//@address : ���ؼ�����ַ
			//@Return : ������
			int createNewServerModule(const std::string address) override;

			//���ٱ��ط����ģ��
			//@Return : ������
			int destroyServerModule(void) override;

		protected:
			const std::string upstreamAddress;
		};//class AbstractUpstreamServer
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_UPSTREAM_SERVER_H
