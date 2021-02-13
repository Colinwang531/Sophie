//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-10
//		Description :					ZMQ��Ϣ��
//
//		History:						Author									Date										Description
//										������									2020-08-10									����
//										������									2020-11-25									1.�޸���Ϣ���������
//																															2.�߼���ʵ�����ݶ��е�FILOģʽ
//

#ifndef FRAMEWORK_NETWORK_ZMQ_MESSAGE_H
#define FRAMEWORK_NETWORK_ZMQ_MESSAGE_H

// #include <vector>
// #include <string>

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			class Msg
			{
			public:
				Msg(void);
				~Msg(void);

			public:
				//�����Ϣ������
				//@msg : ��Ϣ������
				//@Command : 1.���ײ����������
				//			 2.������ݵĹ������̲߳���ȫ��
				void addMessage(const std::string msg);

				//�Ƴ���Ϣ������
				//@Return : ���Ƴ�����Ϣ������
				//@Command : 1.���ײ��Ƴ�����
				//			 2.�Ƴ����ݵĹ������̲߳���ȫ��
				const std::string removeMessage(void);

				//������Ϣ����
				//@s : SOCKET��ʶ
				//@Return : ������
				int recvData(void* s = nullptr);

				//������Ϣ����
				//@s : SOCKET��ʶ
				//@Return : ������
				int sendData(void* s = nullptr);

			private:
				std::vector<std::string> msgDataGroup;
			};//class Msg
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_MESSAGE_H
