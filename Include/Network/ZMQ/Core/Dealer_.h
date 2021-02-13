//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-11
//		Description :					Dealer��
//
//		History:						Author									Date										Description
//										������									2020-08-11									����
//										������									2020-12-28									1.�淶���ֿռ�
//																															2.�ع������
//

#ifndef FRAMEWORK_NETWORK_ZMQ_DEALER_H
#define FRAMEWORK_NETWORK_ZMQ_DEALER_H

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			class Msg;

			class Dealer
			{
			public:
				Dealer(void);
				virtual ~Dealer(void);

			public:
				//�󶨼����˿ں�
				//@s : Socketʵ��
				//@port : �����˿ں�
				//@Return : ������
				int bind(
					void* s = nullptr,
					const unsigned short port = 0);

				//����Զ�̷���
				//@s : Socketʵ��
				//@ipv4 : ����IPv4��ַ
				//@port : ����˿ں�
				//@Return : ������
				int connect(
					void* s = nullptr, 
					const char* ipv4 = nullptr, 
					const unsigned short port = 0);

				//������Ϣ
				//@s : Socketʵ��
				//@msg : ��Ϣ����
				//@Return : ������
				virtual int send(
					void* s = nullptr,
					Msg* msg = nullptr);
			};//class Dealer
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_DEALER_H
