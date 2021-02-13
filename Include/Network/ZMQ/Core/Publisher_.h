//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-11
//		Description :					Publisher��
//
//		History:						Author									Date										Description
//										������									2020-08-11									����
//										������									2021-01-11									1.�淶���ֿռ�
//																															2.�ع������
//

#ifndef FRAMEWORK_NETWORK_ZMQ_PUBLISHER_H
#define FRAMEWORK_NETWORK_ZMQ_PUBLISHER_H

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			class Msg;

			class Publisher
			{
			public:
				Publisher(void);
				virtual ~Publisher(void);

			public:
				//�󶨼����˿ں�
				//@s : Socketʵ��
				//@port : �����˿ں�
				//@hwm : �����С
				//@Return : ������
				int bind(
					void* s = nullptr,
					const unsigned short port = 0,
					const int hwm = -1);

				//������Ϣ
				//@s : Socketʵ��
				//@msg : ��Ϣ����
				//@Return : ������
				virtual int send(
					void* s = nullptr,
					Msg* msg = nullptr);
			};//class Publisher
		}//namespace zeromq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_PUBLISHER_H
