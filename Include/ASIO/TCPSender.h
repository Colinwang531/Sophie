//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-07-14
//		Description:				TCP发送器
//
//		History:						Author									Date													Description
//											王科威										2017-07-14									创建
//

#ifndef ASIO_SENDER_H
#define ASIO_SENDER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "Defs.h"

NS_BEGIN(asio, 2)

typedef boost::function<void(boost::system::error_code, std::size_t)> AfterSendDataNotificationCallback;

class TCPSender : protected boost::enable_shared_from_this<TCPSender>
{
public:
	TCPSender(void);
	~TCPSender(void);

	//	功能 : 数据发送
	//
	//	参数 : 
	//			  @so [IN] Socket实例
	//			  @data [IN] 数据
	//			  @bytes [IN] 字节数
	//			  @callback [IN] 数据发送通知回调函数
	//
	//	返回值 :
	//
	//	备注 :
	//
	void asyncSend(
		boost::asio::ip::tcp::socket* so = nullptr, const char* data = nullptr, const Int32 bytes = 0, AfterSendDataNotificationCallback callback = nullptr);

	//	功能 :				数据发送
	//
	//	参数 :				so																	[in]																Socket实例
	//							data																[in]																数据
	//							bytes															[in]																字节数
	//
	//	返回值 :				错误码
	//
	//	备注 :
	//
// 	virtual Int32 sendPartialData(
// 		boost::asio::ip::tcp::socket* so = nullptr, const char* data = nullptr, const Int32 bytes = 0);
	
private:
	//	功能 :				数据发送通知回调
	//
	//	参数 :				so																	[in]																Socket实例
	//							error																[in]																错误码
	//							streamBuf														[in]																字节流缓存
	//							transfferedBytes											[in]																传输字节数
	//
	//	返回值 :				错误码
	//
	//	备注 :
	//
// 	void afterSendDataNotificationCallback(
// 		boost::asio::ip::tcp::socket* so,
// 		boost::system::error_code error,
// //		boost::shared_ptr<boost::asio::streambuf> streamBuf,
// 		std::size_t transfferredBytes = 0);
// 
// private:
// 	AfterSendDataNotificationCallback sendDataNotificationCallback;
// 	void* sendDataNotificationCtx;
};//class TCPSender

NS_END

#endif//TCP_SENDER_H
