//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-08-10
//		Description :					MQ��Ϣ��
//
//		History:						Author									Date										Description
//										������									2020-08-10									����
//

#ifndef MQ_MESSAGE_MESSAGE_DATA_H
#define MQ_MESSAGE_MESSAGE_DATA_H

#include <vector>
#include <string>
#include "boost/noncopyable.hpp"
#include "socket_base.hpp"

namespace mq
{
    namespace message
    {
		class MessageData : private boost::noncopyable
		{
		public:
			MessageData(void);
			~MessageData(void);

		public:
			//������Ϣ����
			//@s : SOCKET��ʶ
			//@Return : ������
			int recvData(zmq::socket_base_t* s = nullptr);

			//������Ϣ����
			//@s : SOCKET��ʶ
			//@Return : ������
			int sendData(zmq::socket_base_t* s = nullptr);

			//��ȡ��Ϣ���ݸ���
			//@Return : ��Ϣ���ݸ���
			inline const unsigned int getDataCount(void) const
			{
				return static_cast<unsigned int>(dataGroup.size());
			}

			//���ײ������Ϣ����
			//@data : ��������
			//@Return : ������
			int pushFront(const std::string data);

			//��β�������Ϣ����
			//@data : ��������
			//@Return : ������
			int pushEnd(const std::string data);

			//���ײ�ɾ����Ϣ����
			//@Return : ��������
			const std::string popFront(void);

		private:
			//ɾ����Ϣ����������
			void clearup(void);

		private:
			std::vector<std::string> dataGroup;
		};//class MessageData
    }//namespace message
}//namespace mq

#endif//MQ_MESSAGE_MESSAGE_DATA_H
