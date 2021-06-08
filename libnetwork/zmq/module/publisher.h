//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-15
//		Description :					Publisher������
//
//		History:						Author									Date										Description
//										������									2021-01-15									����
//


#ifndef FRAMEWORK_NETWORK_ZMQ_PUB_H
#define FRAMEWORK_NETWORK_ZMQ_PUB_H

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			class Msg;

			class Pub
			{
			public:
				Pub(void);
				virtual ~Pub(void);

			public:
				//����Pub
				//@port : �����˿ں�
				//@Return : ������
				virtual int startPub(const unsigned short port = 0);

				//ֹͣPub
				//@Return : ������
				virtual int stopPub(void);

				//������Ϣ
				//@msg : ��Ϣ����
				//@Return : ������
				int sendMsg(Msg* msg = nullptr);

			protected:
				void* so;
			};//class Pub
		}//namespace zeromq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_PUB_H
