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

#ifndef TCP_CONNECTOR_H
#define TCP_CONNECTOR_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "Defs.h"

NS_BEGIN(asio, 2)

typedef boost::function<void(boost::asio::ip::tcp::socket*, const Int32)> AfterRemoteConnectNotificationCallback;

class TCPConnector : public boost::enable_shared_from_this<TCPConnector>
{
public:
	TCPConnector(
		boost::asio::ip::tcp::socket* so = nullptr,
		AfterRemoteConnectNotificationCallback callback = nullptr);
	virtual ~TCPConnector(void);

	//	���� : �첽����Զ������
	//
	//	���� : 
	//			  @address [IN] ����IPv4��ַ
	//			  @port [IN] �����˿ں�
	//
	//	����ֵ :
	//
	//	��ע :
	//
	void asyncConnect(const char* address = nullptr, const UInt16 port = 60531);

private:
	//	���� : �����¼�����֪ͨ�ص�����
	//
	//	���� : 
	//			  @error [IN] ���������
	//
	//	����ֵ :
	//
	//	��ע : ��ʹ��async_connect���������첽����ʱ,���񵽵�֪ͨû�з���socketʵ��,����޷���socketʵ���������¼���Ӧ����
	//			  Ϊ�ܽ����Ӧ������,��ʱʹ����2���ص���������֪ͨ,���ڿ��޸�ΪЭ�̵ĵ��÷�ʽ
	//
	void afterAsyncConnectNotificationCallback(const boost::system::error_code error);

private:
	boost::asio::ip::tcp::socket* tcpSocket;
	AfterRemoteConnectNotificationCallback afterRemoteConnectNotificationCallback;
};//class TCPConnector

NS_END

#endif//TCP_CONNECTOR_H
