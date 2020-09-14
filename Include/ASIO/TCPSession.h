//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :					������
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-07-14
//		Description:			TCP�Ự
//
//		History:					Author									Date												Description
//										������										2017-07-14								����
//

#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include <deque>
#include "boost/thread/mutex.hpp"
#include "MediaData/MediaData.h"
using MediaData = NS(media, 2)::MediaData;
#include "ASIO/TCPSender.h"
using TCPSender = NS(asio, 2)::TCPSender;
#include "ASIO/TCPReceiver.h"
using TCPReceiver = NS(asio, 2)::TCPReceiver;

NS_BEGIN(asio, 2)

class TCPSession
{
public:
	TCPSession(void);
	virtual ~TCPSession(void);

	Int32 openSession(boost::asio::ip::tcp::socket* so = nullptr);
	void closeSession(void);
	Int32 sendData(const MediaData& data);
	//	���� : ���ݶ�ȡ
	//
	//	���� : 
	//			  @timeo [IN] ���ݽ��ճ�ʱʱ��,��λ��
	//
	//	����ֵ :
	//
	//	��ע :
	//
	void receiveData(const UInt32 timeo = 0);

private:
	void afterAsyncReadDataNotificationCallback(boost::system::error_code error, std::size_t transferredBytes);
	void afterAsyncSendDataNotificationCallback(boost::system::error_code error, std::size_t transferredBytes);

protected:
	virtual Int32 openingSession(boost::asio::ip::tcp::socket* so = nullptr);
	virtual void closingSession(void);
	virtual void asyncSend(const MediaData& data);
	virtual void asyncRecv(
		boost::system::error_code error, std::size_t transferBytes, const char* transferData = nullptr);
	virtual void asyncWait(const UInt32 timeo = 0) = 0;

protected:
	boost::asio::ip::tcp::socket* tcpSocket;
	TCPSender tcpSender;
	TCPReceiver tcpReceiver;

	boost::mutex sendingDataMutex;
	std::deque<MediaData> sendingDataQueue;
	//�ñ�ʶ����socketʵ�����ظ��ر�
	bool stopped;
};//class TCPSession

NS_END

#endif//TCP_SESSION_H
