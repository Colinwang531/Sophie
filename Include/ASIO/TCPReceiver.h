//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-07-14
//		Description:				TCP������
//
//		History:						Author									Date													Description
//											������										2017-07-14									����
//

#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "Timer/Timer.h"
using TimerPtr = boost::shared_ptr<NS(timer, 1)::Timer>;
#include "Defs.h"

NS_BEGIN(asio, 2)

typedef boost::function<void(boost::asio::ip::tcp::socket*, const char*, const UInt32)> AfterReadDataNotificationCallback;

class TCPReceiver : protected boost::enable_shared_from_this<TCPReceiver>
{
public:
	TCPReceiver(AfterReadDataNotificationCallback callback = nullptr);
	~TCPReceiver(void);

	//	���� : ���ݶ�ȡ
	//
	//	���� : 
	//			  @so [IN] Socketʵ��
	//			  @timeo [IN] ��ʱʱ��
	//
	//	����ֵ :
	//
	//	��ע :
	//
	void asyncRead(boost::asio::ip::tcp::socket* so = nullptr, const UInt32 timeo = 0);

private:
	//	���� : ���ݶ�ȡ֪ͨ�ص�
	//
	//	���� : 
	//			  @error [IN] ������
	//			  @transfferedBytes [IN] ���������ֽ���
	//
	//	����ֵ :
	//
	//	��ע :
	//
	void afterAsyncReadDataNotificationCallback(boost::system::error_code error, std::size_t transfferedBytes = 0);

	//	���� : ���ݶ�ȡ��ʱ֪ͨ�ص�
	//
	//	���� : 
	//			  @so [IN] Socketʵ��
	//			  @error [IN] ������
	//
	//	����ֵ :
	//
	//	��ע :
	//
	void afterAsyncReadDataTimeoutNotificationCallback(boost::asio::ip::tcp::socket* so = nullptr, const Int32 error = 0);

private:
	AfterReadDataNotificationCallback afterReadDataNotificationCallback;
	TimerPtr readDataTimeoutPtr;
	bool isReadData;
	char readDataBuffer[gMaxMTUBytes];
};//class TCPReceiver

NS_END

#endif//TCP_RECEIVER_H
