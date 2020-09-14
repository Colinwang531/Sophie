/****************************************************************************************************************************************************
Copyright :				@2017, HHJT, All Rights Reserved

Author :					������
E-mail :					wangkw531@gmail.com
Date :						2017-06-21
Description :			UDP�Ự��
								1.�Ựʹ���첽��ʽ���ͺͽ�������
								2.�Ự����ʹ�ù���ָ�뷽ʽ����

History:					Author									Date												Description
								������									2017-06-21								����
****************************************************************************************************************************************************/


#ifndef UDP_SESSION_H
#define UDP_SESSION_H

#include "Defs.h"

namespace network_1
{
	class UDPSession : public boost::enable_shared_from_this<UDPSession>
	{
	public:
		UDPSession(const UDPSocket* so = NULL);
		virtual ~UDPSession(void);

		//���� :			����UDP�Ự���ݽ���,�÷����ɵ����߸���ʵ��
		//
		//���� :	
		//
		//����ֵ :		������
		virtual int startSession(void) = 0;

		//���� :			ֹͣUDP�Ự���ݽ���
		//
		//���� :	
		//
		//����ֵ :
		virtual void stopSession(void);

		//���� :			��UDP�Ựʵ�������ݽ��ն˿�
		//
		//���� :			portNum							[in]								�˿ں�
		//
		//����ֵ :		������
		virtual int bindReadPort(const boost::uint16_t portNum = gInvalidPort);

		//���� :			��һ���첽���ݶ�ȡ
		//
		//���� :	
		//
		//����ֵ :		������
		virtual int asyncRead(void);

		//���� :			�ɵ�����ʵ�ֵ�UDP�첽���ݶ�ȡ�¼�����
		//					�����߱����ڴ������ȡ�����ݺ���ô˻��෽�����÷����������´�һ���첽���ݶ�ȡ������ûỰ�������ٶ�ȡ������������
		//
		//���� :			udpSenderPtr						[in]							Զ�����ݷ��Ͷ���Ϣ
		//					streamBufPtr						[in]							�����ݻ���
		//					streamBufSize						[in]							�����ݴ�С
		//					errorCode							[in]							��ȡ�¼�������
		//
		//����ֵ :		������
		virtual int asyncReadEventNotify(UDPEndpointPtr udpSenderPtr, StreamBufPtr streamBufPtr, const std::size_t streamBufSize = 0, const ErrorCode& errorCode = ErrorCode());

		//���� :			��һ���첽���ݷ���
		//
		//���� :			address								[in]							Զ�̽��շ�IP��ַ,�汾ΪIPV4
		//					portNum								[in]							Զ�̽��շ��˿ں�
		//					streamBuf							[in]							�����ݻ���
		//					streamBufSize						[in]							�����ݴ�С
		//
		//����ֵ :		������
		virtual int asyncWrite(const char* address = NULL, const boost::uint16_t portNum = gInvalidPort, const char* streamBuf = NULL, const boost::uint32_t streamBufSize = 0);

		//���� :			�ɵ�����ʵ�ֵ�TCP�첽���ݷ����¼�����
		//
		//���� :			streamBufPtr									[in]					�����ݻ���
		//					streamBufSize								[in]					�����ݴ�С
		//					errorCode										[in]					��ȡ�¼�������
		//
		//����ֵ :		������
		virtual int asyncWriteEventNotify(StreamBufPtr streamBufPtr, const std::size_t streamBufSize = 0, const ErrorCode& errorCode = ErrorCode());

	protected:
		UDPSocket* udpSocket;
	};//class UDPSession
}//namespace network_1

#endif//UDP_SESSION_H
