//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					Router��
//
//		History:						Author									Date										Description
//										������									2020-04-26									����
//										������									2020-12-28									1.�淶���ֿռ�
//																															2.�ع������
//

#ifndef FRAMEWORK_NETWORK_ZMQ_ROUTER_H
#define FRAMEWORK_NETWORK_ZMQ_ROUTER_H

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			class Msg;

			class Router
			{
			public:
				Router(void);
				virtual ~Router(void);

			public:
				//�󶨼����˿ں�
				//@s : Socketʵ��
				//@port : �����˿ں�
				//@Return : ������
				virtual int bind(
					void* s = nullptr, 
					const unsigned short port = 0);

				//������Ϣ
				//@s : Socketʵ��
				//@msg : ��Ϣ����
				//@Return : ������
				virtual int send(
					void* s = nullptr,
					Msg* msg = nullptr);
			};//class Router
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_ROUTER_H
