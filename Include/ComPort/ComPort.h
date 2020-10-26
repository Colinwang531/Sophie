//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					������
//
//		History:						Author									Date										Description
//										������									2020-10-03									����
//

#ifndef BASE_COM_COM_PORT_H
#define BASE_COM_COM_PORT_H

#include "boost/asio.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
using ComPortPtr = boost::shared_ptr<boost::asio::serial_port>;

namespace base
{
	namespace com
	{
		typedef boost::function<void(const char*, const int)> ComPortDataNotificationCallback;

		class ComPort
		{
		public:
			ComPort(ComPortDataNotificationCallback callback = nullptr);
			~ComPort(void);

		public:
			//�򿪴���
			//@idx : ���ں�
			//@baudrate : ������
			//@Return : ������
			int openPort(
				const int idx = 0, 
				const int baudrate = 4800);

			//�رմ���
			//@Return : ������
			int closePort(void);

		private:
			void asyncReadDataNotificationCallback(
				ComPortPtr cpp, 
				const boost::system::error_code e,
				const int bytes = 0);
			void processDataReceiveWorkerThread(void);

		private:
			boost::asio::io_service ios;
			ComPortDataNotificationCallback comPortDataNotificationCallback;
			//�������ݻ���
			enum
			{
				eMaxPort = 16,
				eMaxSize = 1//500 * 1024
			};
			char comPortDataCache[eMaxSize];
		};//class ComPort
	}//namespace com
}//namespace base

#endif//BASE_COM_COM_PORT_H
