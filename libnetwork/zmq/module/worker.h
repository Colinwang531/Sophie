//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-07
//		Description :					Dealer模型类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-07									 创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_MODULE_WORKER_H
#define FRAMEWORK_LIBNETWORK_ZMQ_MODULE_WORKER_H

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				class Impl;

				class Worker
				{
				public:
					Worker(void);
					virtual ~Worker(void);

				public:
					//����OD
					//@address : IPv4��ַ
					//@listenPort : �����˿ں�
					//@Return : ������
					virtual int startOD(
						const char* ipv4 = nullptr,
						const unsigned short port = 0);

					//ֹͣOD
					//@Return : ������
					virtual int stopOD(void);

					//������Ϣ
					//@msg : ��Ϣ����
					//@Return : ������
					int sendMsg(Msg* msg = nullptr);

					//��ȡOD����״̬
					//Return : OD����״̬
					inline const bool isStopped(void) const
					{
						return stopped;
					}

				protected:
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
					void* so;
					bool stopped;
				};//class OD
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_MODULE_WORKER_H
