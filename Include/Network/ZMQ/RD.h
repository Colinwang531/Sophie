//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-18
//		Description :					Router-Dealer������
//										2��ģʽ:
//										1.Router��Dealer������,������ַ�;
//										2.Router������Dealer����,���������
//
//		History:						Author									Date										Description
//										������									2020-12-18									����
//										������									2020-12-28									1.�淶���ֿռ�
//																															2.�ع������
//

#ifndef FRAMEWORK_NETWORK_ZMQ_RD_H
#define FRAMEWORK_NETWORK_ZMQ_RD_H

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			class Msg;

			class RD
			{
			public:
				RD(void);
				virtual ~RD(void);

			public:
				//����RD
				//@rlp : Router�����˿ں�
				//@dcip : Dealer����IPv4��ַ
				//@dcp : Dealer���Ӷ˿ں�
				//@Return : ������
				virtual int startRD(
					const unsigned short rlp = 0,
					const char* dcip = nullptr,
					const unsigned short dcp = 0);

				//����RD
				//@rlp : Router�����˿ں�
				//@dlp : Dealer�����˿ں�
				//@Return : ������
				virtual int startRD(
					const unsigned short rlp = 0,
					const unsigned short dlp = 0);

				//ֹͣRD
				//@Return : ������
				virtual int stopRD(void);

				//������Ϣ
				//@idx : Socket������
				//@msg : ��Ϣ����
				//@Return : ������
				int sendMsg(
					const int idx = -1,
					Msg* msg = nullptr);

			protected:
				//Router����Ϣ���մ���
				//@sender : ������ID��ʶ
				//@module : ������ģ�ͱ�ʶ
				//@from : ����������
				//@to : ����������
				//@routers : ��Ϣ·�ɱ�
				//@messages : ��Ϣ����
				virtual void afterParsePolledMessage(
					const std::string sender,
					const std::string module,
					const std::string from,
					const std::string to,
					const std::vector<std::string> routers,
					const std::vector<std::string> messages) = 0;

				//Dealer����Ϣ���մ���
				//@module : ������ģ�ͱ�ʶ
				//@from : ����������
				//@to : ����������
				//@routers : ��Ϣ·�ɱ�
				//@messages : ��Ϣ����
				virtual void afterParsePolledMessage(
					const std::string module,
					const std::string from,
					const std::string to,
					const std::vector<std::string> routers,
					const std::vector<std::string> messages) = 0;

			private:
				//���ݶ�ȡ�߳�
				void pollMessageWorkerThread(void);

				//��Ϣ���մ���
				//Comment : ������Ϣ��ʽ����Ϣ���������ٽ����ϲ㴦��
				void parsePolledMessage(Msg& msg);

			private:
				enum
				{
					SO_NUMBER = 2
				};
				void* sockets[SO_NUMBER];
				bool stopped;
			};//class RD
		}//namespace zeromq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_RD_H
