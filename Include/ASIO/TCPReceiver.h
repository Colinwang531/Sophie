//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-07-14
//		Description:				TCP接收器
//
//		History:						Author									Date													Description
//											王科威										2017-07-14									创建
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

	//	功能 : 数据读取
	//
	//	参数 : 
	//			  @so [IN] Socket实例
	//			  @timeo [IN] 超时时间
	//
	//	返回值 :
	//
	//	备注 :
	//
	void asyncRead(boost::asio::ip::tcp::socket* so = nullptr, const UInt32 timeo = 0);

private:
	//	功能 : 数据读取通知回调
	//
	//	参数 : 
	//			  @error [IN] 错误码
	//			  @transfferedBytes [IN] 传输数据字节数
	//
	//	返回值 :
	//
	//	备注 :
	//
	void afterAsyncReadDataNotificationCallback(boost::system::error_code error, std::size_t transfferedBytes = 0);

	//	功能 : 数据读取超时通知回调
	//
	//	参数 : 
	//			  @so [IN] Socket实例
	//			  @error [IN] 错误码
	//
	//	返回值 :
	//
	//	备注 :
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
