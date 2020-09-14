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

	//	���� : ���ݷ���
	//
	//	���� : 
	//			  @so [IN] Socketʵ��
	//			  @data [IN] ����
	//			  @bytes [IN] �ֽ���
	//			  @callback [IN] ���ݷ���֪ͨ�ص�����
	//
	//	����ֵ :
	//
	//	��ע :
	//
	void asyncSend(
		boost::asio::ip::tcp::socket* so = nullptr, const char* data = nullptr, const Int32 bytes = 0, AfterSendDataNotificationCallback callback = nullptr);

	//	���� :				���ݷ���
	//
	//	���� :				so																	[in]																Socketʵ��
	//							data																[in]																����
	//							bytes															[in]																�ֽ���
	//
	//	����ֵ :				������
	//
	//	��ע :
	//
// 	virtual Int32 sendPartialData(
// 		boost::asio::ip::tcp::socket* so = nullptr, const char* data = nullptr, const Int32 bytes = 0);
	
private:
	//	���� :				���ݷ���֪ͨ�ص�
	//
	//	���� :				so																	[in]																Socketʵ��
	//							error																[in]																������
	//							streamBuf														[in]																�ֽ�������
	//							transfferedBytes											[in]																�����ֽ���
	//
	//	����ֵ :				������
	//
	//	��ע :
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
