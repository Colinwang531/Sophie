//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					TCP�Ự��
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
//

#ifndef BASE_NETWORK_TCP_SESSION_H
#define BASE_NETWORK_TCP_SESSION_H

#include "boost/shared_ptr.hpp"
#include "Packet/Stream/StreamPacket.h"
using StreamPacket = base::packet::StreamPacket;
#include "DataStruct/FIFOQueue.h"
#include "ASIO/TCPSender.h"
using TCPSender = base::network::TCPSender;
using TCPSenderPtr = boost::shared_ptr<TCPSender>;
#include "ASIO/TCPReceiver.h"
using TCPReceiver = base::network::TCPReceiver;
using TCPReceiverPtr = boost::shared_ptr<TCPReceiver>;

namespace base
{
	namespace network
	{
		class TCPSession
		{
		public:
			TCPSession(boost::asio::ip::tcp::socket* s = nullptr);
			virtual ~TCPSession(void);

		public:
			//�����Ự
			//@Return : ������
			//@Comment : �Ự�������Զ���ʼ���ݽ���
			virtual int startSession(void);

			//ֹͣ�Ự
			//@Return : ������
			virtual int stopSession(void);

			//��������
			//@pkt : ���ݰ�
			//@Return : ������
			int sendData(StreamPacket* pkt = nullptr);

			//��ȡ�Ự״̬
			//@Return : �Ự״̬
			inline const bool valid(void) const
			{
				return !stopped;
			}

			//���ݷ���֪ͨ
			//@e : ������
			//@bytes : ���ݴ�С
			virtual void sentdDataNotification(
				const boost::system::error_code e, 
				const int bytes = 0);

			//���ݶ�ȡ֪ͨ
			//@data : ��������
			//@bytes : ���ݴ�С
			virtual void receivedDataNotification(
				const unsigned char* data = nullptr, 
				const int bytes = 0);

			//���ݶ�ȡ�쳣֪ͨ
			//@e : ������
			virtual void receivedExceptionNotification(
				const boost::system::error_code e);

			//���ݶ�ȡ��ʱ֪ͨ
			virtual void receivedExpiredNotification(void);

		private:
			boost::asio::ip::tcp::socket* so;
			//���ⲻ�ϴ������ͺͽ�����
			TCPSenderPtr senderPtr;
			TCPReceiverPtr receiverPtr;
			//Ϊ����ײ㷢�����ݻ��汻д���������
			//�Ự��ʹ�÷��Ͷ��б�֤����һ����һ������
			FIFOQueue<StreamPacket*> streamPacketGroup;
			bool stopped;
		};//class TCPSession
	}//namespace network
}//namespace base

#endif//BASE_NETWORK_TCP_SESSION_H
