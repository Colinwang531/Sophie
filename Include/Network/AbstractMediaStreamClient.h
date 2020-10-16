//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-15
//		Description :					��ý����ͻ�����
//
//		History:						Author									Date										Description
//										������									2020-09-15									����
//

#ifndef BASE_NETWORK_MEDIA_STREAM_CLIENT_H
#define BASE_NETWORK_MEDIA_STREAM_CLIENT_H

#include "boost/shared_ptr.hpp"
#include "boost/system/system_error.hpp"
#include "DataStruct/Vector.h"
#include "DataStruct/UnorderedMap.h"
#include "ASIO/ASIOService.h"
using ASIOService = base::network::ASIOService;
#include "AbstractClient.h"

namespace base
{
	namespace network
	{
		class AbstractMediaStreamClient : public AbstractClient
		{
		public:
			AbstractMediaStreamClient(
				const std::string address, 
				const unsigned short port = 60531);
			virtual ~AbstractMediaStreamClient(void);

		public:
			//���ӷ����
			//@did : �豸ID��ʶ
			//@cid : �����ID��ʶ
			//@idx : ���������
			//@Return : ������
			int connectMediaServerWithID(
				const std::string did, 
				const std::string cid,
				const int idx = -1);

			//�Ͽ����������
			//@did : �豸ID��ʶ
			//@Return : ������
			//@Comment : �Ͽ��豸�����е�����
			int disconnectMediaServer(const std::string did);

		protected:
			//�����ͻ���ģ��
			//@address : ����˵�ַ
			//@name : ���������
			//@Return : ������
			int createNewClientModule(
				const std::string address,
				const std::string name) override;

			//���ٿͻ���ģ��
			//@Return : ������
			int destroyClientModule(void) override;

			virtual int createNewMediaStreamSession(
				const std::string url, 
				boost::asio::ip::tcp::socket* s) = 0;

		private:
			void afterGotRemoteConnectedNotificationCallback(
				boost::asio::ip::tcp::socket* s, const boost::system::error_code e);

		private:
			ASIOService asioService;
			Vector<std::string> urlGroup;
			const std::string mediaStreamIP;
			const unsigned short mediaStreamPort;
		};//class AbstractMediaStreamClient
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_MEDIA_STREAM_CLIENT_H
